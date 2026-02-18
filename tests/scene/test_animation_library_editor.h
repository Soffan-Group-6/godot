/**************************************************************************/
/*  test_animation_library_editor.h                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "scene/resources/animation.h"
#include "scene/resources/animation_library.h"

#include "tests/test_macros.h"

namespace TestAnimationLibraryEditor {

TEST_CASE("[AnimationLibrary] Animation duplicate produces independent copy") {
	Ref<Animation> original = memnew(Animation);
	original->set_length(2.0);
	const int track_index = original->add_track(Animation::TYPE_VALUE);
	original->track_set_path(track_index, NodePath("Node:property"));
	original->track_insert_key(track_index, 0.0, 100);

	Ref<Animation> copy = original->duplicate();

	CHECK(copy.is_valid());
	CHECK(copy != original);
	CHECK(copy->get_length() == doctest::Approx(real_t(2.0)));
	CHECK(copy->get_track_count() == 1);
	CHECK(int(copy->track_get_key_value(0, 0)) == 100);

	// Modifying the copy should not affect the original.
	copy->set_length(5.0);
	CHECK(original->get_length() == doctest::Approx(real_t(2.0)));
	CHECK(copy->get_length() == doctest::Approx(real_t(5.0)));
}

TEST_CASE("[AnimationLibrary] Copy all animations via duplicate preserves entries and independence") {
	Ref<AnimationLibrary> src = memnew(AnimationLibrary);

	Ref<Animation> anim_a = memnew(Animation);
	anim_a->set_length(1.0);
	src->add_animation("walk", anim_a);

	Ref<Animation> anim_b = memnew(Animation);
	anim_b->set_length(2.0);
	src->add_animation("run", anim_b);

	// Copy all animations into a new library using duplicate.
	Ref<AnimationLibrary> dst = memnew(AnimationLibrary);
	List<StringName> animation_list;
	src->get_animation_list(&animation_list);
	for (const StringName &name : animation_list) {
		Ref<Animation> dup = src->get_animation(name)->duplicate();
		dst->add_animation(name, dup);
	}

	CHECK(dst->has_animation("walk"));
	CHECK(dst->has_animation("run"));
	CHECK(dst->get_animation("walk") != src->get_animation("walk"));
	CHECK(dst->get_animation("run") != src->get_animation("run"));
	CHECK(dst->get_animation("walk")->get_length() == doctest::Approx(real_t(1.0)));
	CHECK(dst->get_animation("run")->get_length() == doctest::Approx(real_t(2.0)));

	// Modifying source should not affect destination.
	anim_a->set_length(10.0);
	CHECK(dst->get_animation("walk")->get_length() == doctest::Approx(real_t(1.0)));
}

TEST_CASE("[AnimationLibrary] get_animation returns null for missing key") {
	Ref<AnimationLibrary> lib = memnew(AnimationLibrary);

	Ref<Animation> anim = memnew(Animation);
	lib->add_animation("existing", anim);

	CHECK(lib->get_animation("existing").is_valid());

	ERR_PRINT_OFF;
	CHECK(lib->get_animation("nonexistent").is_null());
	ERR_PRINT_ON;
}

TEST_CASE("[AnimationLibrary] Path classification distinguishes embedded from standalone paths") {
	// Standalone resource file paths start with "res://" and have no "::".
	String standalone_path = "res://animations/walk.tres";
	CHECK(standalone_path.is_resource_file());
	CHECK(standalone_path.find("::") == -1);

	// Embedded scene paths contain "::" indicating a sub-resource within a scene.
	String embedded_path = "res://scenes/player.tscn::Animation_abc123";
	CHECK(embedded_path.find("::") != -1);

	// An embedded path with "::" — extracting the base path before "::".
	int srpos = embedded_path.find("::");
	String base = embedded_path.substr(0, srpos);
	CHECK(base == "res://scenes/player.tscn");

	// A non-resource path (no "res://" prefix).
	String local_path = "Animation_abc123";
	CHECK(!local_path.is_resource_file());
}

} // namespace TestAnimationLibraryEditor
