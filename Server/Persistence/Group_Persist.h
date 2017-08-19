/*************************************************************************
	>    File Name: Persistence/Group_Persist.h
	>       Author: fujie
	>         Mail: fujie.me@qq.com
	> Created Time: 2017年08月18日 星期五 01时27分05秒
 ************************************************************************/

#ifndef _PERSISTENCE/GROUP_PERSIST_H
#define _PERSISTENCE/GROUP_PERSIST_H

//创建群
int Group_Perst_Create(int uid ,const char *name);

//添加成员
int Group_Perst_AddMember(int gid ,int uid);

//删除成员 (踢人或者退群)
int Group_Perst_DeleteMember(int gid ,int uid);

//删除群
int Group_Perst_Delete(int gid);

int Group_Perst_GetMyGroup(group_t *MyGroupList ,int uid);

int Group_Perst_GetGroupMember(group_member_t* GroupMember,int gid);


#endif
