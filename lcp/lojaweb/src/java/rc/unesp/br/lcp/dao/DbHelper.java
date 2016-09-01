/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.dao;

/**
 *
 * @author lfleming
 */

import java.sql.*;

public class DbHelper {

    private static final String jdbcDriver = "com.mysql.jdbc.Driver";
    private static final String dbURL = "jdbc:mysql://localhost:3306/loja";
    private static final String username  = "root";
    private static final String password  = "";

    private Connection connection;

    public DbHelper() throws SQLException, ClassNotFoundException {
    	Class.forName(jdbcDriver);
    	connection = DriverManager.getConnection(dbURL, username, password);
    }

    public ResultSet select(String query)throws SQLException {
        PreparedStatement st  = connection.prepareStatement(query);
        return st.executeQuery();
    }

    public int update(String statement)throws SQLException {
        PreparedStatement st  = connection.prepareStatement(statement);
        return st.executeUpdate();
    }

    public int insert(String query)throws SQLException {
        PreparedStatement st  = connection.prepareStatement(query, Statement.RETURN_GENERATED_KEYS);
        st.executeUpdate();
        ResultSet res = st.getGeneratedKeys();
        if(res.next()) {
                return res.getInt(1);
        } else {
                return 0;
        }
    }

    public void close() {
        try
        {
            connection.close();
        }
        catch (SQLException sqlException)
        {
            sqlException.printStackTrace();
            connection = null;
        }
    }
    
    @Override
    protected void finalize()
    {
        close();
    }
}
