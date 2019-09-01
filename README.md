# 数据库接口表

charset: utf-8
表名: 小写字母下划线隔开
属性名: 驼峰命名, 首字母小写, 属性名第一个单词是表名缩写

### user表

* 属性
 
| 属性名        | 类型         | 备注                                                        | 含义                           |
| ------------- | ------------ | ----------------------------------------------------------- | ------------------------------ |
| userId        | INT UNSIGNED | 主键, 自增=10000, NOT NULL                                  | 用户ID                         |
| userFriNum    | INT          | NOT NULL DEFAULT=0                                          | 好友数量                       |
| userNickName  | VARCHAR(16)  | NOT NULL                                                    | 用户昵称                       |
| userPassword  | VARCHAR(16)  | NOT NULL                                                    | 用户密码                       |
| userSignature | VARCHAR(100) | NOT NULL 可以是''                                           | 用户个性签名                   |
| userStatus    | TINYINT      | NOT NULL DEFAULT=0                                          | 用户在线状态. 0:不在线, 1:在线 |
| userAvatar    | VARCHAR(100) | NOT NULL DEFAULT='img/defaultAvatar.png' 由调用接口的人指定 | 用户头像文件路径               |
| userIP        | INT          | NOT NULL                                                    | 用户ip地址                     |

```sql
CREATE TABLE IF NOT EXISTS linpop.user (
	userId INT UNSIGNED NOT NULL AUTO_INCREMENT,
	userFriNum INT NOT NULL DEFAULT 0,
    userNickName VARCHAR(16) NOT NULL,
    userPassword VARCHAR(16) NOT NULL,
    userSignature VARCHAR(36) NOT NULL,
    userStatus TINYINT DEFAULT 0,
    userAvatar VARCHAR(100) NOT NULL,
    userIp INT NOT NULL,
    PRIMARY KEY(userId)
)DEFAULT CHARSET=utf8 AUTO_INCREMENT=10000;
```
### friends表

所有的好友将以一对一对的形式存在数据库中, 且以friUserId1 < friUserId2来确保每对好友只会出现一次.

* 属性

| 属性名     | 类型          | 备注                              | 含义               |
| ---------- | ------------- | --------------------------------- | ------------------ |
| friId      | INT UNSIGNED  | 主键, 自增                        | 好友对Id           |
| friUserId1 | INT UNSIGNED  | 外键                              | 好友对中一个人的ID |
| friUserId2 | INT  UNSIGNED | 外键, 保证friUserId1 < friUserId2 | 好友对中另一人的ID |

```sql
CREATE TABLE IF NOT EXISTS linpop.friends (
	friId INT UNSIGNED NOT NULL AUTO_INCREMENT,
	friUserId1 INT UNSIGNED NOT NULL,
    friUserId2 INT UNSIGNED NOT NULL,
    PRIMARY KEY(friId),
    FOREIGN KEY(friUserId1) REFERENCES linpop.user(userId),
    FOREIGN KEY(friUserId2) REFERENCES linpop.user(userId)
)DEFAULT CHARSET=utf8;
```
### message表

| 属性名      | 类型     | 备注       | 含义           |
| ----------- | -------- | ---------- | -------------- |
| msgId       | INT      | 主键，自增 | 一对一消息的ID |
| msgContent  | TEXT     | NOT NULL   | 消息的内容     |
| msgDateTime | DATETIME | NOT NULL   | 消息的发送时间 |
| msgStatus   | TINYINT  | NOT NULL   | 消息是否被接收 |
| msgFromId   | INT      | NOT NULL   | 消息发送者的ID |
| msgToId     | INT      | NOT NULL   | 消息接收者的ID |

### group表

| 属性名     | 类型         | 备注       | 含义         |
| ---------- | ------------ | ---------- | ------------ |
| groupId    | INT          | 主键, 自增 | 聊天群的ID   |
| groupName  | VARCHAR(16)  | NOT NULL   | 聊天群的名称 |
| groupIntro | VARCHAR(100) |            | 群介绍       |
| groupIcon  | VARCHAR(100) |            | 群头像       |

### group_user表

群和群成员ID的联系

| 属性名    | 类型 | 备注 | 含义     |
| --------- | ---- | ---- | -------- |
| guGroupId | INT  | 外键 | 群ID     |
| guUserId  | INT  | 外键 | 群成员ID |

### group_message表

群消息表

| 属性名     | 类型     | 备注       | 含义           |
| ---------- | -------- | ---------- | -------------- |
| gmId       | INT      | 主键, 自增 | 群消息ID       |
| gmGroupId  | INT      | 外键       | 群消息所属群   |
| gmContent  | TEXT     | NOT NULL   | 群消息内容     |
| gmFromId   | INT      | 外键       | 群消息发送者ID |
| gmDateTime | DATETIME | NOT NULL   | 群消息发送时间 |