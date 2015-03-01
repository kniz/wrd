#pragma once

#include "../../Includes/Includes.hpp"

namespace DX9Graphics
{
	class NE_DLL DX9 : public NEModule
	{
		typedef DX9 ThisClass;
		typedef NEModule SuperClass;

	public:
#include "ErrorCode.hpp"
#include "Capabilities.hpp"
#include "DisplayMode.hpp"
#include "DisplayModeManager.hpp"
#include "PresentParameters.hpp"

	public:
		NETArgument<NEModuleSelector>	arg_window_binder;
		NETArgument<NEModuleSelector>	arg_cameras;
		NETArgument<NEBooleanKey>		arg_is_vertical_synchronized;
		NETArgument<NEBooleanKey>		arg_is_windowed;
		NETArgument<NEBooleanKey>		arg_is_32bit_color;
		NETArgument<NEBooleanKey>		arg_is_32bit_texture;
		NETArgument<NEIntKey>			arg_screen_width;
		NETArgument<NEIntKey>			arg_screen_height;
		NETArgument<NEFloatKey>			arg_screen_ratio;
		NETArgument<NEIntKey>			arg_back_buffer_count;
		NETArgument<NEIntKey>			arg_screen_reflesh_rate;
		NETArgument<NEIntKey>			arg_swap_effect;

	protected:
		virtual type_result _onFetchArguments(NEArgumentList& tray)
		{
			tray.push(arg_window_binder);
			tray.push(arg_cameras);
			tray.push(arg_is_vertical_synchronized);
			tray.push(arg_is_windowed);
			tray.push(arg_is_32bit_color);
			tray.push(arg_is_32bit_texture);
			tray.push(arg_screen_width);
			tray.push(arg_screen_height);
			tray.push(arg_screen_ratio);
			tray.push(arg_back_buffer_count);
			tray.push(arg_screen_reflesh_rate);
			tray.push(arg_swap_effect);

			return RESULT_SUCCESS;
		}
		virtual type_result _onFetchModule()
		{
			arg_window_binder.getValue().isUsingAutoBinding() = true;
			arg_window_binder.getValue().NENodeSelector::isUsingAutoBinding() = true;
			arg_is_windowed.setValue(true);
			arg_is_32bit_texture.setValue(true);
			arg_is_32bit_color.setValue(true);
			arg_screen_width.setValue(640);
			arg_screen_width.setEnable(false);
			arg_screen_height.setValue(480);
			arg_screen_height.setEnable(false);
			arg_screen_ratio.setValue(640.0f / 480.0f);
			arg_screen_ratio.setEnable(false);
			arg_back_buffer_count.setValue(1);
			arg_screen_reflesh_rate.setValue(60);
			arg_swap_effect.setValue(D3DSWAPEFFECT_DISCARD);

			return RESULT_SUCCESS;
		}
		virtual type_result _onExecute();

	public:
		DX9();
		DX9(const DX9& source);

	public:
		virtual ~DX9();

	public:
		type_result forceWindowHandle(HWND window_handle_to_force); //	윈도우 핸들을 강제한다. 키바인더로 윈도우 핸들을 가져 오지 않게 된다.	대신, 키바인더에서 제공하는 "윈도우 소멸시 자동 리소스 해제"는 불가능하다.
		type_result forcePresentParameters(PresentParameters& parameters_to_force);
		const Capabilities& getCapabilities() const;
		const DisplayModeManager& getDisplayModeManager() const;
		const PresentParameters& getPresentParameters() const;
		DisplayMode getCurrentDisplayMode();
		LPDIRECT3D9 getDirect3D();
		const LPDIRECT3D9 getDirect3D() const;
		LPDIRECT3DDEVICE9 getDevice();
		const LPDIRECT3DDEVICE9 getDevice() const;
		bool isDeviceActive() const;
		bool isKeysForPresentParametersValid() const;
		HWND getHWND() const
		{
			return _window_handle;
		}

	public:
		virtual const NEExportable::ModuleHeader& getHeader() const;

	public:
		virtual NEObject& clone() const;
		virtual void release();
		virtual NEBinaryFileSaver& serialize(NEBinaryFileSaver& saver) const;
		virtual NEBinaryFileLoader& serialize(NEBinaryFileLoader& loader);
		virtual type_result isValid() const;

	public:
		type_result restore();

	private:
		type_result _render();
		type_result _initializeDirects();
		type_result __initializeDirect3D();
		type_result __initializeDevice();
		type_result _releaseDevice();
		type_result _releaseResources();
		type_result _releaseResourcesFromModules();
		type_result _releaseResourcesFromShaderProgram();
		type_result _releaseShaders();
		type_result _restoreResources();
		type_result _restoreShaders();
		type_result _updateTargetWindowSizeWithSpecifiedScreenSize() const;
		void _release();
		type_result _loadKeysFromINI();
		type_result _saveKeysToINI() const;
		const DX9& _assign(const DX9& source);
		type_result _bindWindowHandle();
		type_result __initializeCapabilities();
		const NECodeSet& _getResourceCodes() const;
		const NECodeSet& _getShaderCodes() const;

	private: 
		HWND					_window_handle;
		//	다이렉트:
		LPDIRECT3D9				_direct3d;
		LPDIRECT3DDEVICE9		_device;
		PresentParameters		_present_parameter;
		//	디스플레이 모드:
		DisplayModeManager		_display_mode_manager;
		Capabilities			_capabilities;

	public:
		static HWND global_window;
		static type_index camera_index;
		static DX9& getInstancedDX();

		template <typename T>
		static T& cast(NEModule& target)
		{
			const NECodeSet& cs = T::getModuleScriptCodes();
			T* nullpointer = 0x00;

			NECode sample(target.getScriptCode(), NECodeType(NECodeType::MODULE_SCRIPT));

			if (&target								&&
				cs.find(sample) != NE_INDEX_ERROR)
				return static_cast<T&>(target);

			return *nullpointer;
		}

		template <typename T>
		static T& cast(NETArgument<NEModuleSelector>& target)
		{
			T* nullpointer = 0x00;
			NEModule& module = target.getValue().getModule();
			if (!&module) return *nullpointer;

			target.getValue().initializeReferingPoint();

			return cast<T>(module);
		}

		static D3DXVECTOR3 createYawPitchRollFrom(const D3DXQUATERNION& q)
		{
			float sqw = q.w*q.w;
			float sqx = q.x*q.x;
			float sqy = q.y*q.y;
			float sqz = q.z*q.z;
			D3DXVECTOR3 ret;
			ret.x = DX9::toDegree(-asinf(2.0f * (q.w*q.x - q.y*q.z))); // rotation about x-axis
			ret.y = DX9::toDegree(-atan2f(2.0f * (q.x*q.z + q.w*q.y), (-sqx - sqy + sqz + sqw))); // rotation about y-axis
			ret.z = DX9::toDegree(-atan2f(2.0f * (q.x*q.y + q.w*q.z), (-sqx + sqy - sqz + sqw))); // rotation about z-axis

			return ret;
		}
		static D3DXQUATERNION createQuaternionFrom(type_float yaw, type_float pitch, type_float roll)
		{
			D3DXQUATERNION to_return;
			D3DXQuaternionRotationYawPitchRoll(&to_return, DX9::toRadian(yaw), DX9::toRadian(pitch), DX9::toRadian(roll));

			return to_return;
		}
		static D3DXQUATERNION createQuaternionFrom(const D3DXVECTOR3& direction)
		{
			D3DXVECTOR3 up(0, 1.0f, 0.0f);
			D3DXVECTOR3 right;
			D3DXVec3Cross(&right, &up, &direction);
			D3DXVec3Cross(&up, &direction, &right);

			D3DXMATRIXA16 mBasis(
				right.x, right.y, right.z, 0.0f,
				up.x, up.y, up.z, 0.0f,
				direction.x, direction.y, direction.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			D3DXQUATERNION qrot;
			qrot.w = (float)sqrt(1.0f + mBasis._11 + mBasis._22 + mBasis._33) / 2.0f;
			float dfWScale = qrot.w * 4.0f;
			qrot.x = (float)((mBasis._32 - mBasis._23) / dfWScale);
			qrot.y = (float)((mBasis._13 - mBasis._31) / dfWScale);
			qrot.z = (float)((mBasis._21 - mBasis._12) / dfWScale);

			return qrot;
		}
		static D3DXVECTOR3 createYawPitchRollFrom(const D3DXVECTOR3& direction)
		{
			return createYawPitchRollFrom(createQuaternionFrom(direction));
		}
		static D3DXMATRIX createRotationMatrix(const D3DXQUATERNION& quat)
		{
			D3DXMATRIX to_return;			
			D3DXMatrixRotationQuaternion(&to_return, &quat);

			return to_return;
		}
		static type_float toRadian(type_float degree)
		{
			static const float adj = 3.141592f / 180.0f;

			return adj * degree;
		}
		static type_float toDegree(type_float radian)
		{
			static const float adj = 180.0f / 3.141592f;

			return adj * radian;
		}
		static D3DXVECTOR3 createDirectionVectorFrom(type_float yaw, type_float pitch, type_float roll, const D3DXVECTOR3& cannoncial)
		{
			D3DXMATRIXA16 mat;
			D3DXMatrixRotationQuaternion(&mat, &createQuaternionFrom(yaw, pitch, roll));
			D3DXVECTOR3 to_return;

			D3DXVec3TransformNormal(&to_return, &cannoncial, &mat);
			return to_return;
		}
	};
}