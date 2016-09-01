/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rc.unesp.br.lcp.view;

/**
 *
 * @author lfleming
 */
import java.util.Locale;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

public class Main {

  public static void main(final String[] args) {
     Locale.setDefault(Locale.ENGLISH);
    
    SwingUtilities.invokeLater(new Runnable() {
      @Override
      public void run() {

        final Application app = new Application();
        app.setTitle("Simple Java Database Swing Application");
        app.setSize(400, 300);
        app.setLocationRelativeTo(null);
        app.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        app.setVisible(true);
      }
    });
  }
}
