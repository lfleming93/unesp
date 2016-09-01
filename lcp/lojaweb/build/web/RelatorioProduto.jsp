<%-- 
    Document   : RelatorioProduto
    Created on : Aug 26, 2015, 12:01:17 AM
    Author     : lfleming
--%>

<%@page import="java.util.List"%>
<%@page import="rc.unesp.br.lcp.controller.RelatorioProdutosController"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Relatório de Produtos</title>
        <style>
            table, th, td {
                text-align: center; 
                border: 1px solid black;
            }
        </style>
    </head>
    <body>
        <% 
          RelatorioProdutosController rpc = new RelatorioProdutosController();
          List<Object[]> produtos = rpc.getProdutosRelatorio();
        %>
        <h1>Relatório de Produtos</h1>
        <table>
            <tr>
                <th>Código</th>
                <th>Nome</th>
                <th>Categoria</th>
                <th>Quantidade</th>
                <th>Valor (R$)</th>
                <th>Total Vendido</th>
            </tr>
            <% for(Object[] obj : produtos) { %>
            <tr>
                <td><%=  obj[0].toString() %></td>
                <td><%=  obj[1].toString() %></td>
                <td><%=  obj[2].toString() %></td>
                <td><%=  obj[3].toString() %></td>
                <td><%=  obj[4].toString() %></td>
                <td><%=  obj[5].toString() %></td>
            </tr>
            <% } %>
        </table>
    </body>
</html>
