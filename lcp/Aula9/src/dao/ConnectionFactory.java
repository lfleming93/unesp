/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

/**
 *
 * @author user
 */
public class ConnectionFactory {
    
    public static Connection getConnection(){
        try{
            //Class.forName("com.mysql.jdbc.Driver"); // MYSQL
            Class.forName("org.postgresql.Driver"); // Postgres


            //conexao = DriverManager.getConnection(
            //        "jdbc:mysql://localhost/clientes", "usuario", "senha");
            return DriverManager.getConnection(
                    "jdbc:postgresql://localhost:5432/clientes", "postgres", "123456");                                        
        }
        catch(ClassNotFoundException ex){
            System.out.println("Não foi possível conectar no bd.");
            return null;
        }
        catch(SQLException ex){
            System.out.println("Não foi possível conectar no bd.");
            return null;
        }

    }
    
}
