CREATE DATABASE chat_room CHARACTER SET utf8;

CREATE TABLE user(
    uid int NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT '主键,自增',
    name char(30) COMMENT '用户名',
    password char(32) COMMENT 'MD5加密后的密码'
);







