

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
public interface CmdInterface {
    public void processNewInput(String input, int signature);
    public void processNewError(String error, int signature);
    public void processEnded(int exitValue, int signature);

}
