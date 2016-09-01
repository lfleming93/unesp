<%-- 
    Document   : index
    Created on : Aug 11, 2015, 10:35:31 PM
    Author     : lfleming
--%>

<%@page import="random.Aleatorio"%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Descubra o Número</title>
    </head>
    <body>
        <h1>Descubra o Número</h1>
        <form method="get">
            <div>Digite um número de 1 a 10:</div>
            <input type="text" name="numero" />
            <input type="submit" value="Adivinhar!">
        </form>
        <%
            String param = request.getParameter("numero");
            if (param != null && !param.isEmpty() ) {
              int num = Integer.parseInt(param);
              int rand = Aleatorio.randInt();
              
              if (num == rand) {
        %>
            <div>Acertou o número!!</div>
        <%
              } else {
        %>
        <%= "Errou! O numero correto é: " + rand %>
        <%
              }
            }
        %>
        
        
    </body>
</html>
