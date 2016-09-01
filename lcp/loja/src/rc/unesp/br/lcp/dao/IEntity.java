/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.dao;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.List;
import rc.unesp.br.lcp.beans.Entidade;

/**
 *
 * @author lfleming
 */
abstract class IEntity<T extends Entidade> {
    
    protected String tableName;
    protected DbHelper db;

    public IEntity(String table) {
        this.tableName = table;
        try {
            this.db = new DbHelper();
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }
     
    public List<T> buscarTodos() {
        return buscarTodos("id");
    }
    
    public List<T> buscarTodos(String order) {
        List<T> list = new ArrayList<T>();
        try {
            String select = "select * from " + tableName + " order by " + order;
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
    
    public T buscarPorId(int id) {
        String select = "select * from " + tableName + " where id=" + id;
        
        return buscar(select);
    }
        
    public T buscar(String select) {
        try {
            ResultSet rset = db.select(select);


            if(rset.next()) {
               return converterResultSet(rset);
            }
        }
        catch(Exception ex) {            
            ex.printStackTrace();
        }
        return null;
    }
    
    public T inserir(T entidade) {        
        String select = String.format(
            "insert into %s %s", 
            tableName, converterEntidade(entidade)
        );
          
        try {
            int id = 0;
            id = db.insert(select);
            return buscarPorId(id);
        }
        catch(Exception ex) {            
            ex.printStackTrace();
        }
        
        return null;
    }
    
    public int atualizar(T entidade) {
        String select = String.format(
            "update %s set %s where id=%d",
            tableName, converterEntidadeUpdate(entidade), entidade.getId()
        );
            
        try {
            
            return db.update(select);
            
        }
        catch(Exception ex) {            
            ex.printStackTrace();
        }
        
        return 0;
    }
    
    public int apagar(T entidade) {
        String select = String.format(
            "delete from %s where id=%d",
            tableName, entidade.getId()
        );
            
        try {
            
            db.insert(select);
            
        }
        catch(Exception ex) {            
            ex.printStackTrace();
        }
        
        return 0;
    }
    
    protected abstract T converterResultSet(ResultSet rs);
    
    protected abstract String converterEntidade(T t);
    
    protected abstract String converterEntidadeUpdate(T t);
    
}
