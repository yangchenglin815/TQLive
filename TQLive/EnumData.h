/***********************************************************************************
*								EnumData.h
*
* Copyright(C):	湖南探球网络科技有限公司
*
* Author:  杨承霖
*
* Date: 2019-09
*
* Description: 通用枚举
***********************************************************************************/
#pragma once

//弹幕类型
enum BarrageType
{
	USER_SUSPEND_WARNING = 3500,         //用户停播警告
	USER_TIPS_WARNING    = 3600,         //用户违规提示警告
	USER_LOGOUT_LIVE     = 3801,         //用户退出直播间
	USER_LOGIN_LIVE      = 3802,         //用户登录直播间
	USER_SEND_GIFT       = 7000,         //用户刷礼物
	USER_SEND_MSG        = 8000,         //用户消息
};

//直播间用户角色类型
enum LiveRoomUserRoleType
{
	USERROLE_HOMIE      = 1,           //主播
	USERROLE_ROOMADMIN  = 2,           //房管
	USERROLE_SUPERADMIN = 3,           //超管
};

//IM会话类型
enum TIMConvType
{
	kTIMConv_Invalid = 0,            //无效会话
	kTIMConv_C2C     = 1,            //个人会话
	kTIMConv_Group   = 2,            //群组会话
	kTIMConv_System  = 3,            //系统会话
};

//IM元素类型
enum TIMElemType
{
	kTIMElem_Text          = 0,           //文本元素
	kTIMElem_Image         = 1,           //图片元素
	kTIMElem_Sound         = 2,           //声音元素
	kTIMElem_Custom        = 3,           //自定义元素
	kTIMElem_File          = 4,           //文件元素
	kTIMElem_GroupTips     = 5,           //群组系统消息元素
	kTIMElem_Face          = 6,           //表情元素
	kTIMElem_Location      = 7,           //位置元素
	kTIMElem_GroupReport   = 8,           //群组系统通知元素
	kTIMElem_Video         = 9,           //视频元素
	kTIMElem_FriendChange  = 10,          //关系链表变更消息元素
	kTIMElem_ProfileChange = 11,          //资料变更消息元素
};

//对话框操作类型
enum OperatorType
{
	DIALOG_CANCEL_FORBIDDEN_OPERATOR    = 100,        //解除禁言操作
	DIALOG_DELETE_ROOMMANAGER_OPERATOR  = 101,        //删除房管操作
	DIALOG_PAY_CONFIRM_SIMPLE_OPERATOR  = 102,        //支付确认简易操作
	DIALOG_PAY_CONFIRM_COMPLEX_OPERATOR = 103,        //支付确认复杂操作
	DIALOG_LOGOUT_ACCOUNT_OPERATOR      = 104,        //注销用户操作
	DIALOG_IDENTITY_TIPS_OPERATOR       = 105,        //主播身份提示
	DIALOG_INSTANCE_EXIST_OPERATOR      = 106,        //实例已在运行
	DIALOG_CUSTOM_OPERATOR              = 200,        //自定义内容
};

//应用退出提示类型
enum ExitTipsType
{
	EXIT_APP     = -1000,               //退出应用
	EXIT_TO_TRAY = -1001,               //退出到系统托盘
};

//直播结束类型
enum LiveBroadcastTerminatedType
{
	NORMAL_TERMINATED    = 1000,        //正常中断
	EXCEPTION_TERMINATED = 1001,        //异常中断
	LIVE_CONTENT_RESET = 1002,			//直播内容重置
};

//直播画质类型
enum LivePicQualityType
{
	PICQUALITY_BD_4M     = 0,
	PICQUALITY_BD        = 1,
	PICQUALITY_FHD       = 2,
	PICQUALITY_HD        = 3,
	PICQUALITY_SD        = 4,
	PICQUALITY_CUSTOMIZE = 5
};

//直播码率类型
enum LiveBitrateType
{
	BITRATE_800 = 0,
	BITRATE_1000,
	BITRATE_1200,
	BITRATE_1500,
	BITRATE_2000,
	BITRATE_3000,
	BITRATE_4000
};

//直播帧数类型
enum LiveFrameIntervalType
{
	FRAME_INTERVAL_20 = 0,
	FRAME_INTERVAL_25,
	FRAME_INTERVAL_30,
	FRAME_INTERVAL_40,
	FRAME_INTERVAL_50,
	FRAME_INTERVAL_60
};

//直播分辨率类型
enum LiveResolutionType
{
	RESOLUTION_720P = 0,
	RESOLUTION_450P,
	RESOLUTION_360P,
	RESOLUTION_1080P
};

//编码器类型
enum LiveEncoderType
{
	ENCODER_CPU = 0,
	ENCODER_GPU
};

//输出码率类型
enum LiveOutputBitrateType
{
	OUTPUT_BITRATE_VBR = 0,
	OUTPUT_BITRATE_CBR
};

//登录返回码类型
enum LoginCodeType
{
	LOGIN_ACCOUNT_NULL = -100,
	LOGIN_PASSWD_NULL = -101,
	LOGIN_PASSWD_FORMAT_ERR = -102,
	LOGIN_PHONENUMBER_FORMAT_ERR = -103,
	LOGIN_INITIALIZE = 0,
	LOGIN_CONNECTION_TIMEOUT1 = 417,
	LOGIN_CONNECTION_TIMEOUT2 = 500,
	LOGIN_ACCOUNT_INFO_ERR = 4003,
	LOGIN_FORBIDDEN = 4004,
	LOGIN_REFUSED_BY_POWER = 5005,
	LOGIN_FORBIDDEN_LIMIT = 5006,
	LOGIN_FORBIDDEN_FOREVER = 5009,
	LOGIN_UNABLE_TO_CONNECT_SERVER = 401,
	LOGIN_NETWORK_CONNECTION_ERROR = 5,
	LOGIN_UNKNOWN_ERROR = 99,
};

//素材类型
enum MateriralType
{
	MATERIAL_CAMERA = 1,
	MATERIAL_VIDEO,
	MATERIAL_IMAGE,
	MATERIAL_TEXT,
	MATERIAL_SOURCE,
	MATERIAL_CAPTURE,
	MATERIAL_FULLSCREEN,
	MATERIAL_WINDOW,
};

//源项按钮类型
enum SourceItemButtonType
{
	SOURCEITEM_SETTINGS = 1,
	SOURCEITEM_UP,
	SOURCEITEM_DOWN,
	SOURCEITEM_DEL
};

//播放列表循环类型
enum LoopType
{
	LOOP_RANDOM = 0,
	LOOP_SINGLE = 1,
	LOOP_CYCLE  = 2
};
