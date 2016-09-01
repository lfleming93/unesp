/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aula7;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 *
 * @author user
 */
public class ManipuladorDeEventos implements ActionListener{

    @Override
    public void actionPerformed(ActionEvent e) {
        System.out.println("Me clicaram");
    }
    
}
