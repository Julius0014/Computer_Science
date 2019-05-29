/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package prog;



import java.util.Arrays;
import java.util.Random;



public class prog {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        int x =  Integer.parseInt(args[0]);
        double[] mainarray =  new double[x];
        double[] mainarray2 =  new double[x];
        Random rand = new Random();
        for(int i = 0;i< x;i++){
            mainarray[i] = rand.nextInt(1000) + rand.nextDouble() + 1;
        }
        int n = mainarray.length;
        double[]a = Arrays.copyOfRange(mainarray, 0, (n + 1)/2);
	double[]b = Arrays.copyOfRange(mainarray, (n + 1)/2, n);
        Sortarray[] sortList = new Sortarray[3];
        sortList[0] = new Sortarray(a);
        sortList[1] = new Sortarray(b);
        sortList[2] = new Sortarray(mainarray);
        long start = System.currentTimeMillis();
        thread(sortList);
        double [] s = new double[n]; 
        s = combineArray(s,a,b);
        long end = System.currentTimeMillis();
        long start2 = System.currentTimeMillis();
        onethread(sortList);
        long end2 = System.currentTimeMillis();
         double mil2 = (end - start) ;
         double mil1 = (end2 - start2);
      System.out.printf(" Sorting is done in "+mil2+"ms when two threads are used\n",mil2 );
      System.out.printf(" Sorting is done in " +mil1+"ms when one threads are used\n");
    }
//pthread man array//pthread man array
    private static void onethread(Sortarray[] sortList) {
        Thread object = new Thread(sortList[2]);
        object.start();
        try
        {
            object.join(); 
        } catch(Exception e)
        {
            System.out.println("Interrupted");
        }
    }
//pthread 2 array
    private static void thread(Sortarray[] sortList) {
        Thread object = new Thread(sortList[0]);
        Thread object2 = new Thread(sortList[1]);
        object.start();
        object2.start();
        try
        {
            object.join(); 
            object2.join();
        } catch(Exception e)
        {
            System.out.println("Interrupted"); 
        }   
    }
    public static double[] combineArray(double[] x,double a[],double b[]){
        int i = 0,j=0,k=0;
        while(i != x.length  && j != a.length  && k != b.length  ){
            if(a[j] < b[k]){
                x[i++] = a[j++];
            }else if(a[j] > b[k]){
                 x[i++] = b[k++];
            }else if( a[j] == b[k]){
                x[i++] = a[j++];
            }
        }
        while(a.length != j ) x[i++] = a[j++];
        while(b.length != k ) x[i++] = b[k++];
        return x;
    }

public static class Sortarray extends Thread{
    double myarray[];
    public Sortarray( double[] array){
        this.myarray = array;
       
    } 
    public void run() 
    { 
        try
        {  
            bubbleSort(myarray); 
        } 
        catch (Exception e) 
        {  
            System.out.println ("Exception is caught"); 
        } 
    } 
    void bubbleSort(double arr[]) 
    { 
        int n = arr.length; 
        for (int i = 0; i < n-1; i++) 
            for (int j = 0; j < n-i-1; j++) 
                if (arr[j] > arr[j+1]) 
                { 
                    double temp = arr[j]; 
                    arr[j] = arr[j+1]; 
                    arr[j+1] = temp; 
            }
        } 
    }
}
