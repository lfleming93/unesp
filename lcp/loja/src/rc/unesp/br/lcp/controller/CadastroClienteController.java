/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.controller;

import rc.unesp.br.lcp.beans.Usuario;
import rc.unesp.br.lcp.dao.UsuarioEntity;

/**
 *
 * @author lfleming
 */
public class CadastroClienteController {
  
  public boolean cadastrarOuAtualizarCliente(Usuario u) {
    boolean resultado = false;
    if (! u.getNome().isEmpty() && 
        ! u.getEmail().isEmpty() &&
        ! u.getEndereco().isEmpty() &&
        ! u.getCpf().isEmpty() &&
        ! u.getCelular().isEmpty()) {
      
      UsuarioEntity ue = new UsuarioEntity();
      if (u.getId() == 0) {
        resultado = ue.inserir(u) != null;
      } else {
        resultado = ue.atualizar(u) != 0;
      }
      
    }
    
    return resultado;
  }
  
  public boolean cadastrarAdmin(Usuario u) {
    boolean resultado = false;
    if (! u.getNome().isEmpty() && 
        ! u.getEmail().isEmpty() &&
        ! u.getSenha().isEmpty() &&
        ! u.getCpf().isEmpty()) {
      u.setTipo(1);
      UsuarioEntity ue = new UsuarioEntity();
      resultado = ue.inserir(u) != null;
    }
    
    return resultado;
  }
  
}
