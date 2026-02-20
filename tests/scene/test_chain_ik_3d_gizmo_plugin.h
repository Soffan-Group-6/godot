#pragma once

#include "mocks/mock_chain_ik_3d_gizmo_plugin.h"
#include "mocks/mock_chain_ik_3d_gizmo_plugin.cpp"

/*
#include "scene/resources/mesh.h"
#include "scene/3d/skeleton_3d.h"
#include "scene/3d/skeleton_modifier_3d.h"
#include "core/templates/vector.h"
#include "core/object/ref_counted.h"

#include "core/math/color.h"
#include "core/math/quaternion.h"
#include "core/math/vector3.h"

#include "core/typedefs.h"
#include "core/math/vector3.h"
#include "core/math/transform_3d.h"
*/

#include "tests/test_macros.h"

namespace TestChainIK3DGizmoPlugin {

	TEST_CASE("[Scene][ChainIK3DGizmoPlugin] Test of mock of get_joints_mesh")
	{
		MockChainIK3DGizmoPlugin gizmo;

		Skeleton3D *skeleton = memnew(Skeleton3D);
		int root_bone = skeleton->add_bone("root");
		int child_bone = skeleton->add_bone("child");
		skeleton->set_bone_parent(child_bone, root_bone);
		skeleton->set_bone_rest(root_bone, Transform3D());
		
		MockIterateIK3D *mock_ik = memnew(MockIterateIK3D(1, 2));
		
		bool is_selected = true;

		Ref<MockChainIK3DGizmoPlugin::MockMaterial> selected = memnew(MockChainIK3DGizmoPlugin::MockMaterial);
		Ref<MockChainIK3DGizmoPlugin::MockMaterial> unselected = memnew(MockChainIK3DGizmoPlugin::MockMaterial);

		gizmo.selection_materials.selected_mat = selected;
		gizmo.selection_materials.unselected_mat = unselected;
	
		Ref<ArrayMesh> skinned_mesh;
		skinned_mesh.instantiate();
		Ref<ArrayMesh> mesh;
		mesh.instantiate();
		
		SUBCASE("Default setup, 1 chain, 2 joints")
		{
			gizmo.get_joints_mesh(skeleton, mock_ik, is_selected, skinned_mesh, mesh);

			REQUIRE(skinned_mesh.is_valid());
			REQUIRE(mesh.is_valid());
		}

		SUBCASE("IK chain not selected")
		{
			gizmo.get_joints_mesh(skeleton, mock_ik, false, skinned_mesh, mesh);

			REQUIRE(skinned_mesh.is_valid());
			REQUIRE(mesh.is_valid());

			// Check use of unselected materials
			REQUIRE(gizmo.selection_materials.unselected_mat->albedo == Color(1, 1, 1, 1));
		}
	
		SUBCASE("End bone is extended")
		{
			mock_ik->set_end_bone_extended(0, true);
			mock_ik->set_end_bone_length(0, 1.0f);
			gizmo.get_joints_mesh(skeleton, mock_ik, is_selected, skinned_mesh, mesh);

			REQUIRE(skinned_mesh.is_valid());
			REQUIRE(mesh.is_valid());
		}

		SUBCASE("Bone vector is zero and end bone is extended")
		{
			mock_ik->set_setting_count(1);
			mock_ik->set_joint_count(0, 2);
			mock_ik->set_end_bone_extended(0, true);
			mock_ik->set_end_bone_length(0, 1.0f);

			gizmo.get_joints_mesh(skeleton, mock_ik, is_selected, skinned_mesh, mesh);

			REQUIRE(skinned_mesh.is_valid());
			REQUIRE(mesh.is_valid());
		}
		
		memdelete(mock_ik);
		memdelete(skeleton);		
	}
} //namespace TestChainIK3DGizmoPlugin
