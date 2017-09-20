
/*
 * 稍作修改用于单向链表
 * by fujie
 */




/*
 * list.h
 *
 *  Created on: 2015年4月22日
 *      Author: Administrator
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <assert.h>


//链表必须为单向链表，链表结点必须包含next指针域
/*list 为链表头指针*/
#define List_Init(list, list_node_t) {					\
		list=(list_node_t*)malloc(sizeof(list_node_t)); \
		(list)->next=NULL;					\
	}

//list 为链表头指针，tmpPtr为链表结点临时指针变量
#define List_Free(list, list_node_t) {			\
		assert(NULL!=list);						\
		list_node_t *tmpPtr;					\
		while(NULL!=(tmpPtr=(list)->next)){ 	\
			(list)->next=tmpPtr->next;			\
			free(tmpPtr);						\
		}										\
	}

//list 为链表头指针，tmpPtr为链表结点临时指针变量
#define List_Destroy(list, list_node_t) {		\
		assert(NULL!=list);						\
		List_Free(list, list_node_t)			\
		free(list);								\
		(list)=NULL;							\
	}



//遍历链表
#define List_ForEach(list, curPos) 		\
	 for (   curPos = (list)->next;  	\
		  	  	  curPos != NULL;       \
		  	  	  curPos=curPos->next	\
	    )
	    
	    
	    
//链表头插法，list为头指针，new为新节点
#define List_AddHead(list, newNode) {			\
		(newNode)->next=(list)->next;		 	\
		(list)->next=newNode;				 	\
	}



//链表尾插法，list为头指针，curPos为循环变量 new为新节点
#define List_AddTail(list, curPos ,newNode) {			\
		for(curPos = (list); ;curPos=curPos->next){		\
			if((curPos)->next == NULL){					\
				(curPos)->next = (newNode);				\
				break;									\
			}											\
		}												\
	}

//貌似没用 砍了 好实现 memcpy数据域
//将新节点newNode加入到node之前
#define List_InsertBefore(node, newNode) {		\
		(newNode)->prev=(node)->prev; 		 	\
		(node)->prev->next=newNode;			 	\
		(node)->prev=newNode;			 		\
		(newNode)->next=node;			 		\
	}

//将新节点newNode加入到node之后
#define List_InsertAfter(node, newNode) {		\
		(newNode)->next=node->next;			 	\
		(node)->next=newNode;				 	\
	}

//判断链表是否为空，list为头指针
#define List_IsEmpty(list)  (((list) != NULL)


//删除并释放链表结点node，
#define List_FreeNode(head ,node,list_node_t) {             		\
			assert(NULL!=node);								\
			if(node -> next != NULL){				        \
				list_node_t *temp = node->next;          	\
				memcpy(node,temp,sizeof(list_node_t)); 		\
				free(temp); 								\
			}else{                                          \
                list_node_t *temp = head;                   \
                while(temp -> next != node)                 \
                    temp = temp -> next;                    \
                temp -> next = NULL;                        \
				free(node);		                            \
                node = NULL;                                \
			}												\
	}

#endif
