import ij.plugin.frame.RoiManager;
import ij.ImagePlus;
import ij.IJ;
import ij.gui.ImageWindow;

public class StarryniteLauncher{
	private String imageName;
	private RoiManager roiManager;
	private ImageWindow imageWindow;
	
	
	public StarryniteLauncher(){
			//super("StarryNite");
		ij.ImageJ imagej=new ij.ImageJ();
		imagej.setVisible(false);
			imageName=
				"L:/duz/project/Imaging/ZD_BV36_CDC-25.1_20111030_3/ZD_BV36_CDC-25.1_20111030_3_s1_t1.TIF";
			
		ImagePlus image = IJ.openImage(imageName);
		roiManager=new RoiManager();
		imageWindow=new ImageWindow(image);
		System.out.println(IJ.getToolName());
		IJ.setTool("polygon");
		//this.setImage(image);
		//this.setVisible(true);
	}
	
    public static void main(String[] args) {
      StarryniteLauncher snl=new StarryniteLauncher();
    }
}
