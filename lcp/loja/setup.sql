create database if not exists loja;

use loja;

drop table if exists venda_produtos;
drop table if exists vendas;
drop table if exists produtos;
drop table if exists usuarios;

create table usuarios (
   id int not null auto_increment,
   nome varchar(50),
   email varchar(50),
   endereco varchar(50),
   cpf varchar(50),
   telefone varchar(50),
   celular varchar(50),
   tipo varchar(50),
   senha varchar(50),
   primary key (id));

create table produtos (
  id int not null auto_increment,
  nome varchar(50),
  codigo varchar(50),
  quantidade int,
  categoria varchar(50),
  valor float,
  primary key (id));

create table vendas (
  id int not null auto_increment,
  valor float,
  usuario_id int,
  index user_ind (usuario_id),
  primary key (id),
  foreign key (usuario_id)
      references usuarios(id)
);

create table venda_produtos (
    id int not null auto_increment,
    produto_id int not null,
    venda_id int not null,
    quantidade int,
    valor float,
    primary key(id),
    index (produto_id),
    index (venda_id),

    foreign key (produto_id)
      references produtos(id),

    foreign key (venda_id)
      references vendas(id)
);

insert into usuarios (nome, cpf, tipo, senha) values ('Adminstrador', 'admin', 1, 'admin');
