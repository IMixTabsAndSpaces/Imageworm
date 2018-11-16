

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;

/*
 * Created on Feb 28, 2006
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author biowolp
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class CmdThread implements Runnable {

    // Allocate 1K buffers for Input and Error Streams..
    private byte[] inBuffer = new byte[1024];
    private byte[] errBuffer = new byte[1024];
    // Declare internal variables we will need..
    private Process process;
    private InputStream pErrorStream;
    private InputStream pInputStream;
    private OutputStream pOutputStream;
    private PrintWriter outputWriter;
    private Thread processThread;
    private Thread inReadThread;
    private Thread errReadThread;
    private CmdInterface handler;
    private int iSignature;
    // Private constructor so that no one can create a new ExecHelper directly..
    private CmdThread(CmdInterface ci, Process p, int signature) {
        // Save variables..

        handler = ci;
        process = p;
        iSignature = signature;
        // Get the streams..
        pErrorStream = process.getErrorStream();
        pInputStream = process.getInputStream();
        pOutputStream = process.getOutputStream();
        // Create a PrintWriter on top of the output stream..
        outputWriter = new PrintWriter(pOutputStream, true);
        // Create the threads and start them..
        processThread = new Thread(this);
        inReadThread = new Thread(this);
        errReadThread = new Thread(this);
        // Start Threads..
        processThread.start();
        inReadThread.start();
        errReadThread.start();
    }

    private void processEnded(int exitValue) {
        // Handle process end..
        handler.processEnded(exitValue, iSignature);
    }

    private void processNewInput(String input) {
        // Handle process new input..
        handler.processNewInput(input, iSignature);
    }

    private void processNewError(String error) {
        // Handle process new error..
        handler.processNewError(error, iSignature);
    }

    // Run the command and return the ExecHelper wrapper object..
    public static CmdThread exec(CmdInterface handler, String command, int signature) throws IOException {
        //System.out.println("CmdThread.exec, " + command);
        Process p = Runtime.getRuntime().exec(command);
        return new CmdThread(handler, p, signature);
        //return new CmdThread(handler, Runtime.getRuntime().exec(command), signature);
    }

    // Run the command and return the ExecHelper wrapper object..
    public static CmdThread exec(CmdInterface handler, String command, int signature, File dir) throws IOException {
        //System.out.println("CmdThread.exec, " + command);
        return new CmdThread(handler, Runtime.getRuntime().exec(command, null, dir), signature);
    }

    // Print the output string through the print writer..
    public void print(String output) {
        outputWriter.print(output);
    }

    // Print the output string (and a CRLF pair) through the print writer..
    public void println(String output) {
        outputWriter.println(output);
    }

    public void run() {
        // Are we on the process Thread?
        if (processThread == Thread.currentThread()) {
            try {
                // This Thread just waits for the process to end and notifies the handler..
                processEnded(process.waitFor());
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
            // Are we on the InputRead Thread?
        } else if (inReadThread == Thread.currentThread()) {
            try {
                // Read the InputStream in a loop until we find no more bytes to read..
                for (int i = 0; i > -1; i = pInputStream.read(inBuffer)) {
                    // We have a new segment of input, so process it as a String..
                    processNewInput(new String(inBuffer, 0, i));
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
            // Are we on the ErrorRead Thread?
        } else if (errReadThread == Thread.currentThread()) {
            try {
                // Read the ErrorStream in a loop until we find no more bytes to read..
                for (int i = 0; i > -1; i = pErrorStream.read(errBuffer)) {
                    // We have a new segment of error, so process it as a String..
                    processNewError(new String(errBuffer, 0, i));
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
    }
}
