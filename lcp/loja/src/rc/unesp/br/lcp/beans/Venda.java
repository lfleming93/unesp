/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.beans;

import java.util.ArrayList;
import java.util.List;
import rc.unesp.br.lcp.dao.UsuarioEntity;
import rc.unesp.br.lcp.dao.VendaProdutoEntity;

/**
 *
 * @author lfleming
 */
public class Venda implements Entidade {
    private int id;
    private Usuario cliente;
    private int cliente_id;
    private List<VendaProduto> produtos;
    private float valor;
    
    
    @Override
    public int getId() {
        return this.id;
    }

    @Override
    public void setId(int id) {
        this.id = id;
    }

    public float getValor() {
        if (valor == 0.0) {
            for(VendaProduto vp : produtos) {
                valor += vp.getValor();
            }
        }
        return valor;
    }

    public void setValor(float valor) {
        this.valor = valor;
    }
    
    

    public Usuario getCliente() {
        return cliente;
    }
    
    public void setClienteId(int id) {
      this.cliente_id = id;
    }
    
    public int getClienteId() {
        if (cliente != null) {
            cliente_id = cliente.getId();
        }
        return cliente_id;
    }

    public void setCliente(Usuario cliente) {
        this.cliente = cliente;
        this.cliente_id = cliente.getId();
    }
    
    public void setCliente(int id) {
        UsuarioEntity ue = new UsuarioEntity();
        this.cliente = ue.buscarPorId(id);
    }

    public List<VendaProduto> getProdutos() {
        if (this.produtos == null) {
          VendaProdutoEntity vp = new VendaProdutoEntity();
          this.produtos = vp.buscarTodosPorVenda(this.getId());
        }
        return this.produtos;
    }

    public void setProduto(VendaProduto produto) {
        produto.setVendaId(this.getId());
        if (this.produtos == null) {
          this.produtos = new ArrayList<VendaProduto>();
        }
        this.produtos.add(produto);
    }
    
    
    
}
