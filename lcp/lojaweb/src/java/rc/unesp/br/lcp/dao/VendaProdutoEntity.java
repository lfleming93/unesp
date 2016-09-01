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
import rc.unesp.br.lcp.beans.VendaProduto;

/**
 *
 * @author lfleming
 */
public class VendaProdutoEntity extends IEntity<VendaProduto> {
    public static String tableName = "venda_produtos";

  public VendaProdutoEntity() {
    super(tableName);
  }

    public List<VendaProduto> buscarTodosPorVenda(int venda_id) {
      List<VendaProduto> list = new ArrayList<VendaProduto>();
        try {
            String select = "select * from " + tableName + " where venda_id= " + venda_id;
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
    protected VendaProduto converterResultSet(ResultSet rs) {
        try {
            VendaProduto v = new VendaProduto();
            
            v.setId(rs.getInt("id"));
            
            v.setVendaId(rs.getInt("venda_id"));
            
            v.setProdutoId(rs.getInt("produto_id"));
            
            v.setQuantidade(rs.getInt("quantidade"));
            
            v.setValor(rs.getFloat("valor"));

            return v;
        }
        catch(SQLException sqlEx) {
            sqlEx.printStackTrace();
        }
        
        return null;
        
    }
    
    @Override
    protected String converterEntidade(VendaProduto v) {
        String select = String.format(
            "(venda_id, produto_id, quantidade, valor)" +
            " values (%d, %d, %d, %f)",
            v.getVendaId(),
            v.getProdutoId(),
            v.getQuantidade(),
            v.getValor()
        );
        
        return select;
    }
    
    @Override
    protected String converterEntidadeUpdate(VendaProduto v) {
        String select = String.format(
            "venda_id=%d " +
            "produto_id=%d " +
            "quantidade=%d " +
            "valor=%f ",
            v.getVendaId(),       
            v.getProdutoId(),
            v.getQuantidade()
        );
                
        
        return select;
    }
    
}
