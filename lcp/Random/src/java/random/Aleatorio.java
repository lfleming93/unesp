/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package random;

import java.util.Random;

/**
 *
 * @author lfleming
 */
public class Aleatorio {
  
  public static int randInt() { 

    Random rand = new Random();

    return rand.nextInt(10) + 1;
  }
  
}
