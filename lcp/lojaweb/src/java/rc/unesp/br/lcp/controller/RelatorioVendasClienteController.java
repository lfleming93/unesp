/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.controller;

import java.util.List;
import javax.swing.JComboBox;
import javax.swing.table.DefaultTableModel;
import rc.unesp.br.lcp.beans.Produto;
import rc.unesp.br.lcp.beans.Venda;
import rc.unesp.br.lcp.beans.VendaProduto;
import rc.unesp.br.lcp.dao.VendaEntity;

/**
 *
 * @author lfleming
 */
public class RelatorioVendasClienteController {
  
  public void populaFormulario(int cli_id, DefaultTableModel modelo) {
    VendaEntity ve = new VendaEntity();
    modelo.setRowCount(0);
    
    List<Venda> vendas = ve.buscarTodosPorUsuario(cli_id);
    for (Venda v : vendas) {
      List<VendaProduto> produtos = v.getProdutos();
      for (VendaProduto vp : produtos) {
        Produto p = vp.getProduto();
        modelo.addRow(new Object[] {
          v.getId(),
          p.getCodigo(),
          p.getNome(),
          p.getCategoria(),
          vp.getQuantidade(),
          vp.getValor()
        });
      }
      modelo.addRow(new Object[] {
          "",
          "",
          "",
          "",
          "Total: ",
          v.getValor()
        });
      modelo.addRow(new Object[] {});
      
    }
  }
  
}
