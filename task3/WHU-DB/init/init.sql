use mysql;
ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY 'admin123';
create database test;
use test;
create table user
(
    id int auto_increment primary key,
    username varchar(64) unique not null,
    email varchar(120) unique not null,
    password_hash varchar(128) not null,
    avatar varchar(128) not null
);
insert into user values(1, "bob","bob@aol.com","passwd","avaterpath");
insert into user values(2, "alice","alice@aol.com","passwd","avaterpath");