/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aula9;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author user
 */
public class Aula9 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            //Class.forName("com.mysql.jdbc.Driver"); // MYSQL
            Class.forName("org.postgresql.Driver"); // Postgres
            
            Connection conexao;
            
            //conexao = DriverManager.getConnection(
            //        "jdbc:mysql://localhost/clientes", "usuario", "senha");
            conexao = DriverManager.getConnection(
                    "jdbc:postgresql://localhost:3306/clientes", "postgres", "123456");                    
            
        } catch (ClassNotFoundException ex) {
            System.out.println("Não foi possível conectar no bd");
        }
         catch (SQLException ex) {
            System.out.println("BD não encontrado ou usuário e senha inválidos");
        }        

        
    }
}
