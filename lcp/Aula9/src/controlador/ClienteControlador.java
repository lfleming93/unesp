/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package controlador;

import beans.Cliente;
import dao.ClienteDAO;
import java.sql.SQLException;

/**
 *
 * @author user
 */
public class ClienteControlador {
    
    private ClienteDAO clienteDAO = new ClienteDAO();
    
    public void insereCliente(String nome, String telefone) throws SQLException{
        Cliente cliente = new Cliente();
        cliente.setNome(nome);
        cliente.setTelefone(telefone);
        clienteDAO.insereCliente(cliente);
    }
    
}
