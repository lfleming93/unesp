<%-- 
    Document   : RelatorioCliente
    Created on : Aug 26, 2015, 12:40:45 AM
    Author     : lfleming
--%>

<%@page import="java.util.ArrayList"%>
<%@page import="rc.unesp.br.lcp.beans.Produto"%>
<%@page import="rc.unesp.br.lcp.beans.VendaProduto"%>
<%@page import="rc.unesp.br.lcp.beans.Venda"%>
<%@page import="java.util.List"%>
<%@page import="rc.unesp.br.lcp.dao.VendaEntity"%>
<%@page import="rc.unesp.br.lcp.beans.Usuario"%>
<%@page import="rc.unesp.br.lcp.dao.UsuarioEntity"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Relatorio de Clientes</title>
        <style>
            table, th, td {
                text-align: center; 
                border: 1px solid black;
            }
        </style>
    </head>
    <body>
        <% 
            UsuarioEntity ue = new UsuarioEntity(); 
        List<Object[]> objetos = new ArrayList<Object[]>();
            int cli_id = 0;
            if (request.getParameter("id") != null) {
                VendaEntity ve = new VendaEntity();
                cli_id =  Integer.parseInt(request.getParameter("id"));
                List<Venda> vendas = ve.buscarTodosPorUsuario(cli_id);
                for (Venda v : vendas) {
                  List<VendaProduto> produtos = v.getProdutos();
                  for (VendaProduto vp : produtos) {
                    Produto p = vp.getProduto();
                    objetos.add(new Object[] {
                      v.getId(),
                      p.getCodigo(),
                      p.getNome(),
                      p.getCategoria(),
                      vp.getQuantidade(),
                      vp.getValor()
                    });
                  }
                  objetos.add(new Object[] {
                      "",
                      "",
                      "",
                      "",
                      "Total: ",
                      v.getValor()
                    });
                  objetos.add(new Object[] {"", "", "", "", "", ""});

                }
            }
        %>
        <h1>Relatorio de Clientes</h1>
        
        <form action="/lojaweb/RelatorioCliente.jsp" method="GET">
            <select name="id">
                <% for (Usuario u : ue.buscarTodosClientes()) { %>
                <option value="<%= u.getId() %>"><%= u.getNome()  %></option>
                <% } %>
            </select>
            <input type="submit" value="Enviar" />
        </form>
            
        <br>
        
        <table>
            <tr>
                <th>Venda</th>
                <th>Código</th>
                <th>Produto</th>
                <th>Categoria</th>
                <th>Quantidade</th>
                <th>Valor (R$)</th>
            </tr>
            <% for(Object[] obj : objetos) { %>
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
