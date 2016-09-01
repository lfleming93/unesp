/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.dao;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import rc.unesp.br.lcp.beans.Produto;
import rc.unesp.br.lcp.beans.Venda;
import rc.unesp.br.lcp.beans.VendaProduto;

/**
 *
 * @author lfleming
 */
public class VendaEntity extends IEntity<Venda> {
    public static String tableName = "vendas";

  public VendaEntity() {
    super(tableName);
  }
    
    @Override
    public Venda inserir(Venda v) {
      Venda vendaSalva = super.inserir(v);
      
      ProdutoEntity pe = new ProdutoEntity();
      VendaProdutoEntity vpEntity = new VendaProdutoEntity();
      for (VendaProduto vp : v.getProdutos()) {
        Produto p = vp.getProduto();
        
        vp.setVendaId(vendaSalva.getId());
        vendaSalva.setProduto(vpEntity.inserir(vp));
        
        p.setQuantidade(p.getQuantidade() - vp.getQuantidade());
        pe.atualizar(p);
        
      }
      return vendaSalva;
    }

    @Override
    public int atualizar(Venda v) {
      int result = super.atualizar(v);
      if (result != 0 ) {
        VendaProdutoEntity vpEntity = new VendaProdutoEntity();
        for (VendaProduto vp : v.getProdutos()) {
          vpEntity.atualizar(vp);
        }
      }
      return result;
    }
        
    @Override
    protected Venda converterResultSet(ResultSet rs) {
        try {
            Venda v = new Venda();
            
            v.setId(rs.getInt("id"));
            
            v.setCliente(rs.getInt("usuario_id"));
            
            v.setValor(rs.getFloat("valor"));

            return v;
        }
        catch(SQLException sqlEx) {
            sqlEx.printStackTrace();
        }
        
        return null;
        
    }
    
    public List<Venda> buscarTodosPorUsuario(int usuario_id) {
      List<Venda> list = new ArrayList<Venda>();
        try {
            String select = "select * from " + tableName + " where usuario_id= " + usuario_id;
            ResultSet rset = db.select(select);


            while(rset.next()) {
               list.add(converterResultSet(rset));
            }
        }
        catch(Exception ex) {            
            ex.printStackTrace();
        }
        return list;
    }
    
    @Override
    protected String converterEntidade(Venda v) {
        String select = String.format(
            "(usuario_id, valor)" +
            " values (%d, %f)",
            v.getClienteId(),
            v.getValor()
        );
        
        return select;
    }
    
    @Override
    protected String converterEntidadeUpdate(Venda v) {
        String select = String.format(
            "usuario_id=%d " +
            "valor=%f ",
            v.getClienteId(),
            v.getValor()
        );
                
        
        return select;
    }
    
}
