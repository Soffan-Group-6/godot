/**
 * Mock classes and structs containing the bare bone content required to
 * unit test the mock of ChainIK3DGizmoPlugin::get_joints_mesh.
 *
 * @file mock_chain_ik_3d_gizmo_plugin.h
 */

#pragma once

#include "scene/resources/mesh.h"
#include "scene/3d/skeleton_3d.h"
#include "scene/3d/skeleton_modifier_3d.h"
#include "core/templates/vector.h"
#include "core/object/ref_counted.h"
#include "core/math/color.h"
#include "core/math/quaternion.h"
#include "core/math/vector3.h"

struct MockSurfaceTool;
struct MockChainIK3D;

/**
 * Bare bone mock of ChainIK3DGizmoPlugin to support unit testing of the mock
 * of ChainIK3DGizmoPlugin::get_joints_mesh. 
 */
class MockChainIK3DGizmoPlugin
{
	public:
		void get_joints_mesh(Skeleton3D *p_skeleton, MockChainIK3D *p_ik, bool p_is_selected,
			                 Ref<ArrayMesh> &r_skinned_mesh, Ref<ArrayMesh> &r_mesh);

		/**
		 * Bare bone mock of Material to support unit testing of the mock of
		 * ChainIK3DGizmoPlugin::get_joints_mesh.
		 */
		struct MockMaterial : public RefCounted {
			Color albedo;

			void set_albedo(const Color &c) {
				albedo = c;
			}
		};

		/**
		 * Bare bone mock of SelectionMaterials to support unit testing of the mock
		 * of ChainIK3DGizmoPlugin::get_joints_mesh.
		 */
		struct MockSelectionMaterials {
			Ref<MockMaterial> selected_mat;
			Ref<MockMaterial> unselected_mat;
		};

		static MockSelectionMaterials selection_materials;

		void draw_line(Ref<MockSurfaceTool> &p_surface_tool, const Vector3 &p_begin_pos,
				       const Vector3 &p_end_pos, const Color &p_color);

};

/**
 * Bare bone mock of ChainIK3D to support unit testing of the mock of
 * ChainIK3DGizmoPlugin::get_joints_mesh.
 */
struct MockChainIK3D {
	struct BoneJoint {
		int dummy = 0;
	};

	Vector<Vector<BoneJoint>> joints;

	Vector<bool> end_bone_extended;
	Vector<float> end_bone_length;

	void set_setting_count(int count) {
		joints.resize(count);
		end_bone_extended.resize(count);
		end_bone_length.resize(count);
	}

	int get_setting_count() const {
		return joints.size();
	}

	void set_joint_count(int p_index, int p_count) {
		if (p_index >= joints.size()) {
			joints.resize(p_index + 1);
			end_bone_extended.resize(p_index + 1);
			end_bone_length.resize(p_index + 1);
		}
		joints.write[p_index].resize(p_count);
	}

	int get_joint_count(int p_index) const {
		if (p_index >= joints.size()) {
			return 0;
		}
		return joints[p_index].size();
	}

	void set_end_bone_extended(int p_index, bool extended) {
		end_bone_extended.write[p_index] = extended;
	}

	bool is_end_bone_extended(int p_index) const {
		return end_bone_extended[p_index];
	}

	void set_end_bone_length(int p_index, float length) {
		end_bone_length.write[p_index] = length;
	}

	float get_end_bone_length(int p_index) const {
		return end_bone_length[p_index];
	}

	Transform3D get_chain_root_global_rest(int p_index) const {
		return Transform3D();
	}

	int get_joint_bone(int p_index, int p_joint) const {
		return 0;
	}

	Vector3 get_bone_vector(int p_index, int p_joint) const {
		return Vector3();
	}
};
/*
struct MockChainIK3D {
	struct BoneJoint {
		int dummy = 0;
	};

	Vector<Vector<BoneJoint>> joints;

	void set_setting_count(int count) {
		joints.resize(count);
	}

	int get_setting_count() const {
		return joints.size();
	}

	void set_joint_count(int p_index, int p_count) {
		if (p_index >= joints.size()) {
			joints.resize(p_index + 1);
		}
		joints.write[p_index].resize(p_count);
	}

	int get_joint_count(int p_index) const {
		if (p_index >= joints.size()) {
			return 0;
		}

		return joints[p_index].size();
	}
};
*/

/**
 * Bare bone mock of SurfaceTool to support unit testing of the mock of
 * ChainIK3DGizmoPlugin::get_joints_mesh.
 */
struct MockSurfaceTool : public RefCounted {
	Ref<MockChainIK3DGizmoPlugin::MockMaterial> last_material;
	Color last_color;

	void set_material(const Ref<MockChainIK3DGizmoPlugin::MockMaterial> &mat) {}
	void set_bones(const PackedInt32Array &bones) {}
	void set_weights(const PackedFloat32Array &weights) {}
	void set_color(const Color &c) {}
	void add_vertex(const Vector3 &v) {}

	Ref<ArrayMesh> commit() {
		Ref<ArrayMesh> mesh;
		mesh.instantiate();
		return mesh;
	}
};

/**
 * Bare bone mock of JointLimitation3D to support unit testing of the mock
 * of ChainIK3DGizmoPlugin::get_joints_mesh.
 */
struct MockJointLimitation3D : public RefCounted
{
	bool is_valid() const
	{
		return true;
	}

	void draw_shape(Ref<MockSurfaceTool> &st, const Transform3D &tr, float scale,
		            const Color &color) {}
};

/**
 * Bare bone mock of IterateIK3D to support unit testing of the mock of
 * ChainIK3DGizmoPlugin::get_joints_mesh.
 */
struct MockIterateIK3D : public MockChainIK3D
{
	Vector<Vector<SkeletonModifier3D::RotationAxis>> axes;
	Vector<Vector<Ref<MockJointLimitation3D>>> joint_limitations;

	MockIterateIK3D(int p_chain_count, int p_joint_count) {
		set_setting_count(p_chain_count);

		axes.resize(p_chain_count);
		joint_limitations.resize(p_chain_count);

		for (int i = 0; i < p_chain_count; i++) {
			axes.write[i].resize(p_joint_count);
			//joint_limitations.write[i].resize(p_joint_count);
			Vector<Ref<MockJointLimitation3D>> &chain_vec = joint_limitations.write[i];
			chain_vec.resize(p_joint_count);

			for (int j = 0; j < p_joint_count; j++) {
				chain_vec.write[j] = Ref<MockJointLimitation3D>(memnew(MockJointLimitation3D));
			}
			set_joint_count(i, p_joint_count);
		}
	}

	SkeletonModifier3D::RotationAxis get_joint_rotation_axis(int p_index, int p_joint) const
	{
		return axes[p_index][p_joint];
	}

	Ref<MockJointLimitation3D> get_joint_limitation(int p_index, int p_joint) const
	{
		return joint_limitations[p_index][p_joint];
	}

	Vector3 get_joint_rotation_axis_vector(int p_index, int p_joint) const
	{
		return Vector3();
	}

	Quaternion get_joint_limitation_space(int p_index, int p_joint, const Vector3 &p_forward) const
	{
		return Quaternion();
	}
};
