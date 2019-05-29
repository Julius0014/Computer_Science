package cs3743;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Date;

public class P3Program 
{
    private Connection connect = null;

    private Statement statement = null;
    private PreparedStatement preparedStatement = null;
    private ResultSet resultSet = null;
    public static final int ER_DUP_ENTRY = 1062;
    public static final int ER_DUP_ENTRY_WITH_KEY_NAME = 1586;
    public static final String[] strFlightIdM =
    {   "510"
        ,"705"
            ,"331"
            ,"END"
    };

    public P3Program (String user, String password) throws Exception
    {
        try
        {
            // This will load the MySQL driver, each DBMS has its own driver
            Class.forName("com.mysql.jdbc.Driver");
            this.connect = DriverManager.getConnection
                ("jdbc:mysql://10.100.1.81:3306/ubh226db"
                 , user
                 , password);
        }
        catch (Exception e) 
        {
            throw e;
        } 
    }

    public void i2MoreRev(){
        try{

            statement = connect.createStatement();
            resultSet =   statement.executeQuery("  select count(f.flightId),f.flightId from Reservation f group by f.flightId having count(f.flightId) > 2");
            MySqlUtility.printUtility("Flights Having more than 2 reservations",resultSet);
        }catch(SQLException e2) {

        }



    }



    public void OtherCustomers(int k){
        try{

            statement = connect.createStatement();
            resultSet =   statement.executeQuery("select c.name,c.custNr, r3.flightId from Customer c, Customer c2, Reservation r3  where  r3.custNr = c2.custNr and c.custNr != \"1999\" and c2.custNr = \"1999\"  and not exists( select r.flightId from Reservation r where c.custNr = r.custNr and not exists( select * from Reservation r2 where r.flightId = r2.flightId ))");
            if( k == 1 ){
                MySqlUtility.printUtility("Other customers on my flights",resultSet);
            }else{
                MySqlUtility.printUtility("Other customers on my flights after mine were deleted",resultSet);
            }


        } catch (SQLException e2){
            // throw e2;
        }



    }

    public void insertReservation() throws SQLException {
        try{
            preparedStatement = connect.prepareStatement("insert into Reservation values" +  
                    "(?,?,?)");
            for(int i =0;i < strFlightIdM.length-1;i++){
                preparedStatement.setString(1,"1999");
                preparedStatement.setString(2,strFlightIdM[i].toString());
                preparedStatement.setInt(3,2);
                preparedStatement.executeUpdate(); 
            } 
        }catch (SQLException e2) 
        {
            switch(e2.getErrorCode()){
                case ER_DUP_ENTRY:
                case ER_DUP_ENTRY_WITH_KEY_NAME:
                    System.out.printf("Duplicate key error: %s\n",e2.getMessage());
                    break;
                default:
                    throw e2;
            }
        }catch(Exception e2){
            throw e2;
        }

        preparedStatement = connect.prepareStatement("SELECT *  from Reservation where custNr = " + "?" );
        preparedStatement.setString(1,"1999");
        resultSet = preparedStatement.executeQuery(); 
        MySqlUtility.printUtility("My reservations",resultSet);

    }
    public  void insertCustomer() throws SQLException{
        try{

            statement = connect.createStatement();
            statement.executeUpdate("insert into Customer "
                    + "( `custNr`,`name`,`preferAirline`,`birthDt`,`gender`)"
                    + "values(\"1999\",\"Julius Jenkins\", \"Delta\",\"1992-09-09\", \"M\")");
        }catch (SQLException e2) 
        {
            switch(e2.getErrorCode()){
                case ER_DUP_ENTRY:
                case ER_DUP_ENTRY_WITH_KEY_NAME:
                    System.out.printf("Duplicate key error: %s\n",e2.getMessage());
                    break;
                default:
                    throw e2;
            }
        }
        catch(Exception e2){
            throw e2;
        }
    }
    public void runProgram() throws Exception 
    {
        ResultSet resultSet = null;
        try 
        {
            preparedStatement = connect.prepareStatement("SELECT c.* from Customer c");
            resultSet = preparedStatement.executeQuery(); 
            printCustomers("Beginning Customers",resultSet);
            preparedStatement = connect.prepareStatement("SELECT f.* from Flight f");
            resultSet = preparedStatement.executeQuery(); 
            MySqlUtility.printUtility("Beginning Flights",resultSet);
            
            insertCustomer();
            preparedStatement = connect.prepareStatement("SELECT c.* from Customer c");
            resultSet = preparedStatement.executeQuery(); 
            
            
            printCustomers("Customers after I was added",resultSet);
            insertReservation();
            OtherCustomers(1);
            statement = connect.createStatement();
            resultSet =   statement.executeQuery("  select count(f.flightId),f.flightId from Reservation f group by f.flightId having count(f.flightId) > 2");
            MySqlUtility.printUtility("Flights Having more than 2 reservations",resultSet);
            statement = connect.createStatement();
            statement.executeUpdate("Delete from Reservation where custNr = \"1999\"");
            OtherCustomers(2);
        } 
        catch (Exception e) 
        {
            throw e;
        } 
        finally 
        {
            close();
        }

    }                                                                                                                        

    private void printCustomers(String title, ResultSet resultSet) throws SQLException 
    {
        System.out.printf("%s\n", title);
        System.out.printf("%-8s %-20s %-20s %-12s %-4s\n","CustNr","Name","preferAirline","birthDt","gender");
        while(resultSet.next()){
            int custNr = resultSet.getInt("custNr");
            String name  = resultSet.getString("name");
            String  preferAirline = resultSet.getString("preferAirline");          // can be null
            String  birthDt  = resultSet.getString("birthDt");          // can be null
            String  gender =  resultSet.getString("gender");          // can be null
            if ( preferAirline == null)
                preferAirline = "---";
            if (birthDt == null)
                birthDt = "---";
            System.out.printf("%-8s %-20s %-20s %-12s %-4s\n"
                    , custNr
                    , name
                    , preferAirline
                    , birthDt
                    , gender);
        }
    }

    // Close the resultSet, statement, preparedStatement, and connect
    private void close() 
    {
        try 
        {
            if (resultSet != null) 
                resultSet.close();

            if (statement != null) 
                statement.close();

            if (preparedStatement != null) 
                preparedStatement.close();

            if (connect != null) 
                connect.close();
        } 
        catch (Exception e) 
        {

        }
    }

}
