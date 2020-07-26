// INTERPRETER.java

import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.*;

public class INTERPRETER{

	private static final int DATA_SEG_SIZE = 100;
	static String studentName = "Joowon Suh";
	static String studentID = "44414081";
	static String uciNetID = "joowons";

	private LineNumberReader codeIn;
	private LineNumberReader inputIn;
	public  FileOutputStream outFile;
	private PrintStream fileData;

	private ArrayList<String> C;
	private int[] D;
	private int PC;
	private String IR;
	private boolean run_bit;

	public static void main(String[] args)
	{
		INTERPRETER i = new INTERPRETER(args[0]);
		i.runProgram();
	}

	public void runProgram(){

		// TODO FETCH-INCREMENT-EXECUTE CYCLE
		while(run_bit != false){
			fetch();
			incrementPC();
			execute();
		}


		printDataSeg();
	}

	///// helper function and variables /////
	private void printCodeSeg(){
		C.forEach( (n) -> System.out.println( n ) );

	}




	////////////////////////////

	private void printDataSeg() {
		fileData.println("Data Segment Contents");
		for(int i=0; i < DATA_SEG_SIZE; i++){
			fileData.println(i+": "+D[i]);
		}
	}

	private void fetch() {
		// TODO Auto-generated method stub
	 	// System.out.println(C.toArray()[PC]);
		IR = C.toArray()[PC].toString();
	}

	private void incrementPC() {
		PC++;
		// TODO Auto-generated method stub
	}

	private void execute() {
		// TODO Auto-generated method stub
		parseStatement(IR);
	}

	public INTERPRETER(String sourceFile) {
		try {
			inputIn = new LineNumberReader(new FileReader("input.txt"));
			inputIn.setLineNumber(1);

			outFile = new FileOutputStream(sourceFile + ".out");
			fileData = new PrintStream(outFile);
		} catch (IOException e) {
			e.printStackTrace();
			System.err.println("Init: Errors accessing input.txt "
					+ sourceFile);
			System.exit(-2);
		}

		//Initialize the SIMPLESEM processor state
		try {
			//Initialize the Code segment
			C = new ArrayList<String>();
			codeIn = new LineNumberReader(new FileReader(sourceFile));
			codeIn.setLineNumber(1);
			while(codeIn.ready()){
				C.add(codeIn.readLine().replaceAll("\\s", ""));
			}
			//Initialize the Data segment
			D = new int[DATA_SEG_SIZE];
			for(int i=0;i<DATA_SEG_SIZE;i++)
				D[i]=0;
			} catch (IOException e) {
				e.printStackTrace();
				System.err.println("Init: Errors accessing source file " + sourceFile);
				System.exit(-2);
			}
			PC = 0; //Every SIMPLESEM program begins at instruction 0
			IR = null;
			run_bit = true;//Enable the processor
	}

   //Output: used in the case of: set write, source
	private void write(int source){
		fileData.println(source);
	}

	//Input: used in the case of: set destination, read
	private int read(){
		int value=Integer.MIN_VALUE;
		try{value = new Integer((inputIn.readLine())).intValue();}catch(IOException e){e.printStackTrace();}
		return value;
	}

	private void parseStatement(String statement) {
		System.err.println("Statement");
		if(statement.matches("halt.*"))
		{
			run_bit = false;
			return;
		}
		else if(statement.matches("set.*"))
		{
			parseSet(statement);
		}
		else if(statement.matches("jumpt.*")){
			parseJumpt(statement);
		}
		else if(statement.matches("jump.*")){
			parseJump(statement);
		}

	}

	private void parseSet(String statement){
		System.err.println("Set");
		String expr1, expr2;
		int e1, e2;
		Matcher m;
		(m = Pattern.compile("set([^,]*),(.*)").matcher(statement)).find();
		expr1 = m.group(1);
		expr2 = m.group(2);

		if(!Pattern.matches("write.*", expr1)){
			e1 = Integer.parseInt(parseExpr(expr1));
			System.out.println(e1);
			if(!Pattern.matches("read.*", expr2)){
				System.out.println(expr2);
				e2 = Integer.parseInt(parseExpr(expr2));
				D[e1] = e2;
			}
			else{
				System.out.println(expr2);
				D[e1] = read();
				System.out.println("D[0]"+D[0]);
			}
		}
		else{
			if(Pattern.matches("D\\[.*", expr2)){
				String temp = expr2;
				int count = 0;
				boolean nested = true;
				while(nested){
					System.out.println("set expr2 "+expr2);
					(m = Pattern.compile("D\\[(.*)").matcher(expr2)).find();
					expr2 = m.group(1);
					(m = Pattern.compile("(.*)\\]").matcher(expr2)).find();
					expr2 = m.group(1);
					System.out.println("set expr2 "+expr2);
					// System.out.println("expr2"+expr2);
					// e2 = Integer.parseInt(expr2);
					// System.out.println("expr3"+expr3);
					count++;
				}
			}else{
				e2 = Integer.parseInt(parseExpr(expr2));
				write(e2);
			}
		}
	}
	private String parseExpr(String expr) {
		System.err.println("Expr");
		String remainder;
		Matcher m;
					// System.out.println(expr);
		expr = parseTerm(expr);

		while(Pattern.matches("[+-].*", expr)){
			(m = Pattern.compile("[+-](.*)").matcher(expr)).find();
			expr = parseTerm(m.group(1));
		}
		return expr;
	}


	private String parseTerm(String expr) {
		System.err.println("Term");
		String remainder;
		Matcher m;
					// System.out.println(expr);
		expr = parseFactor(expr);

		while(Pattern.matches("[\\*\\/\\%].*", expr)){
			(m = Pattern.compile("[\\*\\/\\%](.*)").matcher(expr)).find();
			expr = parseFactor(m.group(1));
		}
		return expr;
	}


	private String parseFactor(String expr) {
		System.err.println("Factor");
		Matcher m;

		if(Pattern.matches("D\\[.*", expr))
		{

			(m = Pattern.compile("D\\[(.*)").matcher(expr)).find();
			String temp = parseExpr(m.group(1));
			(m = Pattern.compile("\\](.*)").matcher(expr)).find();
			expr = m.group(1);
		}
		else if(Pattern.matches("\\(.*", expr)){
			(m = Pattern.compile("\\((.*)").matcher(expr)).find();

			expr = parseExpr(m.group(1));
			(m = Pattern.compile("\\)(.*)").matcher(expr)).find();
			expr = m.group(1);
		}
		else if(Pattern.matches("\\d*.*", expr)){
			// System.out.println(expr);
			(m = Pattern.compile("(\\d*)(.*)").matcher(expr)).find();
			expr = m.group(1);
			// System.out.println(expr);

		}
		return expr;
	}


	private void parseJump(String statement){
		System.err.println("Jump");
		String expr;
		Matcher m;
		(m = Pattern.compile("jump(.*)").matcher(statement)).find();
		expr = m.group(1);
		parseExpr(m.group(1));
		PC = Integer.parseInt(expr)-1;

	}
	//<Jumpt>-> jumpt <Expr>, <Expr> ( != | == | > | < | >= | <= ) <Expr>
	private void parseJumpt(String statement){
		System.err.println("Jumpt");
		String expr1, expr2, expr3, condition;
		int e1, e2 = 0, e3;
		Matcher m;
		(m = Pattern.compile("jumpt([^,]*),(.*)").matcher(statement)).find();
		expr1 = m.group(1);
		expr2 = m.group(2);

		e1 = Integer.parseInt(parseExpr(expr1));

		(m = Pattern.compile("([^=!><]*)([=!><]*)(.*)").matcher(expr2)).find();
		expr2 = m.group(1);
		condition = m.group(2);
		expr3 = m.group(3);
		parseExpr(expr2);
		parseExpr(expr3);

		if(Pattern.matches("D\\[.*", expr2)){
			// System.out.println("condition"+condition);
			// System.out.println("expr2"+expr2);
			(m = Pattern.compile("D\\[(.*)").matcher(expr2)).find();
			expr2 = parseExpr(m.group(1));
			// System.out.println("expr2"+expr2);
			e2 = Integer.parseInt(expr2);
			// System.out.println("expr3"+expr3);
		}
		e3 = Integer.parseInt(expr3);
		if(D[e2] == e3){
			PC = e1-1;
		}
	}
}
