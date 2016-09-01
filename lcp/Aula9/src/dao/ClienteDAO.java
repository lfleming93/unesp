/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package dao;

import beans.Cliente;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

/**
 *
 * @author user
 */
public class ClienteDAO {
    
    private Connection conexao = ConnectionFactory.getConnection();
    
    public void insereCliente(Cliente cliente) throws SQLException{
        String query = "Insert into clientes (nome, telefone) values (?, ?)";
        PreparedStatement stmt = conexao.prepareStatement(query);
        stmt.setString(1, cliente.getNome());
        stmt.setString(2, cliente.getTelefone());
        stmt.executeUpdate();
        stmt.close();
    }
    
    public void excluiCliente(Cliente cliente) throws SQLException{
        String query = "Delete from clientes where id = ?";
        PreparedStatement stmt = conexao.prepareStatement(query);
        stmt.setInt(0, cliente.getId());
        stmt.executeUpdate();
        stmt.close();                
    }
    
    public void atualizaCliente(Cliente cliente) throws SQLException{
        String query = "Update clientes set nome = ?, telefone = ? where id = ?";
        PreparedStatement stmt = conexao.prepareStatement(query);
        stmt.setString(1, cliente.getNome());
        stmt.setString(2, cliente.getTelefone());
        stmt.setInt(3, cliente.getId());
        stmt.executeUpdate();
        stmt.close();            
    }
    
    public ArrayList<Cliente> consultaCliente(String nome){
            ArrayList<Cliente> listaCliente = new ArrayList<Cliente>();
            String query = "Select * from clientes where nome = ?";
            PreparedStatement stmt = conexao.prepareStatement(query);
            stmt.setString(1, nome);
            ResultSet rs = stmt.executeQuery();

            while (rs.next()){
                Cliente cliente = new Cliente();
                cliente.setId(rs.getInt("id"));
                cliente.setNome(rs.getString("nome"));
                cliente.setTelefone(rs.getString("telefone"));
                
                listaCliente.add(cliente);
            }
            stmt.close();        
            return listaCliente;
        
    }
    
}
