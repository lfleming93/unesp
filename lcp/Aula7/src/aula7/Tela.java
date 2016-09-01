/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aula7;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 *
 * @author user
 */
public class Tela {
    
    JButton botao3 = new JButton("Botão 3");           

/*public class ManipuladorDeEventosInner implements ActionListener{

    @Override
    public void actionPerformed(ActionEvent e) {        
        botao3.setText("Clicaram no me clique!");
    }
    
}      

public class ManipuladorDeEventosInner2 implements ActionListener{

    @Override
    public void actionPerformed(ActionEvent e) {        
        botao3.setText("Clicaram no botão debaixo!");
    }
    
}*/
    
    public void constroiTela(){
        JFrame janela = new JFrame("Título da minha janela");
        janela.setVisible(true);
        janela.setSize(200, 400);
        janela.setDefaultCloseOperation(janela.EXIT_ON_CLOSE);
        
        JButton botao = new JButton("Me clique!");
        janela.getContentPane().add(botao, BorderLayout.NORTH);
        
        
           
        JButton botao2 = new JButton("Botão debaixo!");
        janela.getContentPane().add(botao2, BorderLayout.SOUTH);
        //com classes internas
        //botao2.addActionListener(new ManipuladorDeEventosInner2());
        botao2.addActionListener(new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent e) {
                botao3.setText("Cliquei no botão debaixo");
            }
            
        });
        
        JPanel panel = new JPanel();
        janela.getContentPane().add(panel, BorderLayout.CENTER);
        
        
        panel.add(botao3, BorderLayout.EAST);        
        //com classes internas
        //botao.addActionListener(new ManipuladorDeEventosInner());
        botao.addActionListener(new ActionListener(){

            @Override
            public void actionPerformed(ActionEvent e) {
                botao3.setText("Cliquei no botão de cima");
            }
            
        });
        
        /*while (true){
            System.out.println("Estou executando");
        }*/        
    }
    
}
