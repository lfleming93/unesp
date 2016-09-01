/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.beans;

import rc.unesp.br.lcp.dao.ProdutoEntity;
import rc.unesp.br.lcp.dao.VendaEntity;

/**
 *
 * @author lfleming
 */
public class VendaProduto implements Entidade {
    private int id;
    private int quantidade;
    private Produto produto;
    private Venda venda;
    private double valor;
    private int produtoId;
    private int vendaId;
    
    
    @Override
    public int getId() {
        return this.id;
    }

    @Override
    public void setId(int id) {
        this.id = id;
    }

    public int getQuantidade() {
        return quantidade;
    }

    public void setQuantidade(int quantidade) {
        this.quantidade = quantidade;
    }

    public Produto getProduto() {
        if (this.produto == null) {
          ProdutoEntity pe = new ProdutoEntity();
          this.produto = pe.buscarPorId(this.produtoId);
        }
          
        return this.produto;
    }

    public void setProduto(Produto produto) {
        this.produto = produto;
        this.produtoId = produto.getId();
    }
    
    public void setProduto(int id) {
        ProdutoEntity pe = new ProdutoEntity();
        this.produto = pe.buscarPorId(id);
    }
    
    public Venda getVenda() {
        if (this.venda == null) {
          VendaEntity ve = new VendaEntity();
          this.venda = ve.buscarPorId(this.vendaId);
        }
          
        return this.venda;
    }

    public void setVenda(Venda venda) {
        this.venda = venda;
        this.vendaId = venda.getId();
    }
    
    public void setVenda(int id) {
        VendaEntity ve = new VendaEntity();
        this.venda = ve.buscarPorId(id);
    }

    public double getValor() {
        if (this.valor == 0.0) {
            this.valor = this.getProduto().getValor() * quantidade;
        }
        
        return this.valor;
    }
    
    public void setValor(double valor) {
      this.valor = valor;
    }

    public int getProdutoId() {
        return produtoId;
    }

    public void setProdutoId(int produtoId) {
        this.produtoId = produtoId;
    }

    public int getVendaId() {
        return this.vendaId;
    }

    public void setVendaId(int vendaId) {
       this.vendaId = vendaId;
    }
       
}
