/*************************************************************************
	>    File Name: Persistence/Group_Persist.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月18日 星期五 01时27分05秒
 ************************************************************************/

#ifndef _GROUP_PERSIST_H
#define _GROUP_PERSIST_H
#include "../Service/Group_Srv.h"
//群名是否存在
int Group_Perst_IsGroup(const char *gname);

//创建群
int Group_Perst_Create(int uid ,const char *name);

//添加成员
int Group_Perst_AddMember(int gid ,int uid);

//删除成员 (踢人或者退群)
int Group_Perst_DeleteMember(int gid ,int uid);

//删除群 同时删除群成员
int Group_Perst_Delete(int gid);

int Group_Perst_GetMyGroup(group_t *MyGroupList ,int uid);

int Group_Perst_GetGroupMember(group_member_t* GroupMember,int gid);

group_t * Group_Perst_GetInfo(int gid);


#endif
