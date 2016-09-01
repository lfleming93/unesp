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
public class LoginController {
    
    public boolean AutenticarUsuario(String cpf, String senha) {
        UsuarioEntity ue = new UsuarioEntity();
        Usuario u = ue.buscarPorCpf(cpf);
        
        return u != null && u.getTipo() == 1 && u.getSenha().equals(senha);
    }
    
}
