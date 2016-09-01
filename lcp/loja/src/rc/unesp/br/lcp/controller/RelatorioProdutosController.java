/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.controller;

import java.util.AbstractList;
import java.util.ArrayList;
import java.util.List;
import rc.unesp.br.lcp.beans.Produto;
import rc.unesp.br.lcp.dao.ProdutoEntity;

/**
 *
 * @author lfleming
 */
public class RelatorioProdutosController {
  
  public List<Object[]> getProdutosRelatorio() {
    ProdutoEntity pe = new ProdutoEntity();
    List<Produto> produtos = pe.buscarTodos("categoria");
    
    return getProdutos(produtos);
  }

  public List<Object[]> getProdutosRelatorio(String categoria) {
    ProdutoEntity pe = new ProdutoEntity();
    List<Produto> produtos = pe.buscarProCategoria(categoria);

    return getProdutos(produtos);
  }
  
  protected List<Object[]> getProdutos(List<Produto> produtos) {
    List<Object[]> result = new ArrayList<>();
    for (Produto p : produtos) {
      result.add(new Object[]{
        p.getCodigo(),
        p.getNome(),
        p.getCategoria(),
        p.getQuantidade(),
        p.getValor(),
        p.getQtdVendido()
      });
    }
    
    return result;
  }
  
}
