/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.beans;

import rc.unesp.br.lcp.dao.ProdutoEntity;

/**
 *
 * @author lfleming
 */
public class Produto implements Entidade {
    private int id;
    private String nome;
    private String codigo;
    private int quantidade;
    private String categoria;
    private double valor;
    
    
    @Override
    public int getId() {
        return this.id;
    }

    @Override
    public void setId(int id) {
        this.id = id;
    }
    
    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public String getCodigo() {
        return codigo;
    }

    public void setCodigo(String codigo) {
        this.codigo = codigo;
    }

    public int getQuantidade() {
        return quantidade;
    }

    public void setQuantidade(int quantidade) {
        this.quantidade = quantidade;
    }

    public String getCategoria() {
        return categoria;
    }

    public void setCategoria(String categoria) {
        this.categoria = categoria;
    }

    public double getValor() {
        return valor;
    }
    
    public int getQtdVendido() {
      ProdutoEntity ue = new ProdutoEntity();
      return ue.quantidadeVendido(this.id);
    }

    public void setValor(double valor) {
        this.valor = valor;
    }
}
