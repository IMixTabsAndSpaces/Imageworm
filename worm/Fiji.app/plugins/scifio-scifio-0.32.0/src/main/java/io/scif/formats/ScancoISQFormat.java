
package io.scif.formats;

import io.scif.*;
import io.scif.config.SCIFIOConfig;
import io.scif.io.RandomAccessInputStream;
import io.scif.util.FormatTools;

import java.io.IOException;
import java.time.Instant;
import java.time.LocalDate;
import java.time.ZoneId;
import java.time.ZoneOffset;

import net.imagej.axis.Axes;
import net.imagej.axis.DefaultLinearAxis;

import org.scijava.plugin.Plugin;

/**
 * The file format reader for Scanco ISQ files
 * <p>
 * ISQ images are generated by Scanco X-ray microtomography scanners
 * </p>
 * <p>
 * Scanco ISQ Header Information:
 * </p>
 * <ul>
 * <li>Scanco uses OpenVMS on Alpha workstations</li>
 * <li>Little endian byte order (the least significant bit occupies the lowest
 * memory position)</li>
 * </ul>
 * <p>
 * 
 * <pre>
 * BYTE
 * 00   char    check[16]; // CTDATA-HEADER_V1
 * 16   int     data_type;
 * 20   int     nr_of_bytes;
 * 24   int     nr_of_blocks;
 * 28   int     patient_index;
 * 32   int     scanner_id;
 * 36   int     creation_date[2];
 * 44   int     dimx_p;
 * 48   int     dimy_p;
 * 52   int     dimz_p;
 * 56   int     dimx_µm;
 * 60   int     dimy_µm;
 * 64   int     dimz_µm;
 * 68   int     slice_thickness_µm;
 * 72   int     slice_increment_µm;
 * 76   int     slice_1_pos_µm;
 * 80   int     min_data_value;
 * 84   int     max_data_value;
 * 88   int     mu_scaling;
 * 92   int     nr_of_samples;
 * 96   int     nr_of_projections;
 * 100  int     scandist_µm;
 * 104  int     scanner_type;
 * 108  int     sampletime_µs;
 * 112  int     index_measurement;
 * 116  int     site;
 * 120  int     reference_line_µm;
 * 124  int     recon_alg;
 * 128  char    name[40];
 * 168  int     energy; // V
 * 172  int     intensity; // µA
 * ...
 * 508  int     data_offset; // Number of optional header blocks
 * </pre>
 *
 * @author B. Koller (SCANCO Medical AG, April 2005)
 * @author K.-H. Kunzelmann (Operative Dentistry, LMU-München, Ger, April 2006)
 * @author Michael Doube (Royal Veterinary College, London)
 * @author Richard Domander (Royal Veterinary College, London)
 */
@Plugin(type = Format.class, name = "Scanco ISQ")
public class ScancoISQFormat extends AbstractFormat {

	/** The header byte sequence which identifies Scanco ISQ files */
	public static final String ISQ_ID = "CTDATA-HEADER_V1";

	@Override
	protected String[] makeSuffixArray() {
		return new String[] { "isq" };
	}

	public static class Checker extends AbstractChecker {

		@Override
		public boolean suffixSufficient() {
			return false;
		}

		@Override
		public boolean isFormat(final RandomAccessInputStream stream)
			throws IOException
		{
			byte[] firstBytes = new byte[ISQ_ID.length()];
			stream.read(firstBytes);
			final String fileStart = new String(firstBytes);
			return ISQ_ID.equals(fileStart);
		}
	}

	public static class Metadata extends AbstractMetadata {

		/**
		 * The unit of the spatial calibration, e.g. "voxel size"
		 * <p>
		 * NB (Default)UnitService recognises "µm" as "um"
		 * </p>
		 */
		public static final String UNIT = "um";

		/** Bytes in an ISQ header block */
		public static final int HEADER_BLOCK = 512;
		/**
		 * January 1 1970 as a VMS timestamp, i.e. in 100 nanosecond ticks from
		 * November 17, 1858
		 */
		public static final long UNIX_EPOCH = 0x007c_9567_4beb_4000L;

		@Field(label = "Patient index")
		private int patientIndex;
		@Field(label = "Scanner ID")
		private int scannerId;
		@Field(label = "Creation date")
		private LocalDate creationDate;
		@Field(label = "Slice width in pixels")
		private int width;
		@Field(label = "Slice height in pixels")
		private int height;
		@Field(label = "Slices in stack")
		private int slices;
		@Field(label = "Physical width (µm)")
		private int physicalWidth;
		@Field(label = "Physical height (µm)")
		private int physicalHeight;
		@Field(label = "Physical depth (µm)")
		private int physicalDepth;
		@Field(label = "Slice thickness (µm)")
		private int sliceThickness;
		@Field(label = "Slice spacing (µm)")
		private int sliceSpacing;
		@Field(label = "Location of first slice in sample (µm)")
		private int firstSlicePosition;
		@Field(label = "Minimum value in the data")
		private int minDataValue;
		@Field(label = "Maximum value in the data")
		private int maxDataValue;
		@Field(label = "X-ray linear attenuation value scaling constant")
		private int muScaling;
		@Field(label = "Number of samples")
		private int samples;
		@Field(label = "Number of projections")
		private int projections;
		@Field(label = "The physical distance the stack covers (µm)")
		private int scanDistance;
		@Field(label = "Scanner type")
		private int scannerType;
		@Field(label = "Time the sampling took (µs)")
		private int samplingTime;
		@Field(label = "Index of measurement")
		private int measurementIndex;
		@Field(label = "Sample site (coded value)")
		private int site;
		@Field(label = "Length of the reference line (µm)")
		private int referenceLine;
		@Field(label = "Reconstruction algorithm (coded value)")
		private int reconstructionAlgorithm;
		@Field(label = "Patient name")
		private String patientName;
		@Field(label = "Energy (V)")
		private int energy;
		@Field(label = "Intensity (µA)")
		private int intensity;
		@Field(label = "Starting byte of image data")
		private int dataOffset = HEADER_BLOCK;
		private double voxelWidth;
		private double voxelHeight;
		private double voxelDepth;
		/** Number of bytes in a single slice of the image */
		private int sliceBytes;

		public LocalDate getCreationDate() {
			return creationDate;
		}

		public int getDataOffset() {
			return dataOffset;
		}

		public int getEnergy() {
			return energy;
		}

		public int getFirstSlicePosition() {
			return firstSlicePosition;
		}

		public int getHeight() {
			return height;
		}

		public int getIntensity() {
			return intensity;
		}

		public int getMaxDataValue() {
			return maxDataValue;
		}

		public int getMeasurementIndex() {
			return measurementIndex;
		}

		public int getMinDataValue() {
			return minDataValue;
		}

		public int getMuScaling() {
			return muScaling;
		}

		public int getPatientIndex() {
			return patientIndex;
		}

		public String getPatientName() {
			return patientName;
		}

		public int getPhysicalDepth() {
			return physicalDepth;
		}

		public int getPhysicalHeight() {
			return physicalHeight;
		}

		public int getPhysicalWidth() {
			return physicalWidth;
		}

		public int getProjections() {
			return projections;
		}

		public int getReconstructionAlgorithm() {
			return reconstructionAlgorithm;
		}

		public int getReferenceLine() {
			return referenceLine;
		}

		public int getSamples() {
			return samples;
		}

		public int getSamplingTime() {
			return samplingTime;
		}

		public int getScanDistance() {
			return scanDistance;
		}

		public int getScannerId() {
			return scannerId;
		}

		public int getScannerType() {
			return scannerType;
		}

		public int getSite() {
			return site;
		}

		public int getSliceSpacing() {
			return sliceSpacing;
		}

		public int getSliceThickness() {
			return sliceThickness;
		}

		public int getSlices() {
			return slices;
		}

		public int getWidth() {
			return width;
		}

		public int getSliceBytes() { return sliceBytes; }

		/**
		 * Converts the given timestamp to a creation date
		 * <p>
		 * Timestamp starts from byte 36 of the file
		 * </p>
		 *
		 * @param vmsTimestamp An eight byte VMS timestamp
		 */
		public void setCreationDate(final long vmsTimestamp) {
			/* Unix timestamps mark seconds from the 1-Jan-1970 i.e. the Unix epoch.
			 * VMS timestamps mark 100 ns ticks from 17-Nov-1858.
			 * To convert from VMS to Unix you first need the work out the difference
			 * of the VMS timestamp to the Unix epoch. Then you divide the result
			 * with 10 000 000 (100 ns to seconds)
			 */
			long unixTimestamp = (vmsTimestamp - UNIX_EPOCH) / 10_000_000;
			this.creationDate = Instant.ofEpochSecond(unixTimestamp).atZone(ZoneId
				.ofOffset("", ZoneOffset.UTC)).toLocalDate();
		}

		/**
		 * Sets the starting point of image date in the file
		 * <p>
		 * The 16-bit optional header number is found at byte 508 in the file.
		 * </p>
		 *
		 * @param optionalHeaders Number of optional header blocks
		 */
		public void setDataOffset(final int optionalHeaders) {
			this.dataOffset = (optionalHeaders + 1) * HEADER_BLOCK;
		}

		public void setEnergy(final int energy) {
			this.energy = energy;
		}

		public void setFirstSlicePosition(final int firstSlicePosition) {
			this.firstSlicePosition = firstSlicePosition;
		}

		public void setHeight(final int height) {
			this.height = height;
		}

		public void setIntensity(final int intensity) {
			this.intensity = intensity;
		}

		public void setMaxDataValue(final int maxDataValue) {
			this.maxDataValue = maxDataValue;
		}

		public void setMeasurementIndex(final int measurementIndex) {
			this.measurementIndex = measurementIndex;
		}

		public void setMinDataValue(final int minDataValue) {
			this.minDataValue = minDataValue;
		}

		public void setMuScaling(final int muScaling) {
			this.muScaling = muScaling;
		}

		public void setPatientIndex(final int patientIndex) {
			this.patientIndex = patientIndex;
		}

		public void setPatientName(final String patientName) {
			this.patientName = patientName;
		}

		public void setPhysicalDepth(final int physicalDepth) {
			this.physicalDepth = physicalDepth;
		}

		public void setPhysicalHeight(final int physicalHeight) {
			this.physicalHeight = physicalHeight;
		}

		public void setPhysicalWidth(final int physicalWidth) {
			this.physicalWidth = physicalWidth;
		}

		public void setProjections(final int projections) {
			this.projections = projections;
		}

		public void setReconstructionAlgorithm(final int reconstructionAlgorithm) {
			this.reconstructionAlgorithm = reconstructionAlgorithm;
		}

		public void setReferenceLine(final int referenceLine) {
			this.referenceLine = referenceLine;
		}

		public void setSamples(final int samples) {
			this.samples = samples;
		}

		public void setSamplingTime(final int samplingTime) {
			this.samplingTime = samplingTime;
		}

		public void setScanDistance(final int scanDistance) {
			this.scanDistance = scanDistance;
		}

		public void setScannerId(final int scannerId) {
			this.scannerId = scannerId;
		}

		public void setScannerType(final int scannerType) {
			this.scannerType = scannerType;
		}

		public void setSite(final int site) {
			this.site = site;
		}

		public void setSliceSpacing(final int sliceSpacing) {
			this.sliceSpacing = sliceSpacing;
		}

		public void setSliceThickness(final int sliceThickness) {
			this.sliceThickness = sliceThickness;
		}

		public void setSlices(final int slices) {
			this.slices = slices;
		}

		public void setWidth(final int width) {
			this.width = width;
		}

		// TODO Populate calibration metadata when it's supported
		@Override
		public void populateImageMetadata() {
			createImageMetadata(1);
			final ImageMetadata metadata = get(0);
			metadata.setLittleEndian(true);
			metadata.setBitsPerPixel(16);
			metadata.setPixelType(FormatTools.INT16);
			metadata.setOrderCertain(true);
			metadata.setPlanarAxisCount(2);
			setVoxelDimensions();
			metadata.setAxes(new DefaultLinearAxis(Axes.X, UNIT, voxelWidth),
				new DefaultLinearAxis(Axes.Y, UNIT, voxelHeight), new DefaultLinearAxis(
					Axes.Z, UNIT, voxelDepth));
			metadata.setAxisLengths(new long[] { width, height, slices });
			setSliceBytes();
			// TODO add calibration function 1 / muScaling 1/cm
		}

		private void setSliceBytes() {
			final ImageMetadata imageMetadata = get(0);
			if (imageMetadata == null) {
				return;
			}
			final int bytesPerPixel = imageMetadata.getBitsPerPixel() / 8;
			sliceBytes = width * height * bytesPerPixel;
		}

		private void setVoxelDimensions() {
			voxelWidth = 1.0 * physicalWidth / width;
			voxelHeight = 1.0 * physicalHeight / height;
			voxelDepth = 1.0 * physicalDepth / slices;
		}
	}

	public static class Parser extends AbstractParser<Metadata> {

		@Override
		protected void typedParse(final RandomAccessInputStream stream,
			final Metadata meta, final SCIFIOConfig config) throws IOException,
			FormatException
		{
			config.imgOpenerSetComputeMinMax(true);

			stream.order(true);
			stream.seek(28);
			meta.setPatientIndex(stream.readInt());
			meta.setScannerId(stream.readInt());
			meta.setCreationDate(stream.readLong());
			meta.setWidth(stream.readInt());
			meta.setHeight(stream.readInt());
			meta.setSlices(stream.readInt());
			meta.setPhysicalWidth(stream.readInt());
			meta.setPhysicalHeight(stream.readInt());
			meta.setPhysicalDepth(stream.readInt());
			meta.setSliceThickness(stream.readInt());
			meta.setSliceSpacing(stream.readInt());
			meta.setFirstSlicePosition(stream.readInt());
			meta.setMinDataValue(stream.readInt());
			meta.setMaxDataValue(stream.readInt());
			meta.setMuScaling(stream.readInt());
			meta.setSamples(stream.readInt());
			meta.setProjections(stream.readInt());
			meta.setScanDistance(stream.readInt());
			meta.setScannerType(stream.readInt());
			meta.setSamplingTime(stream.readInt());
			meta.setMeasurementIndex(stream.readInt());
			meta.setSite(stream.readInt());
			meta.setReferenceLine(stream.readInt());
			meta.setReconstructionAlgorithm(stream.readInt());
			meta.setPatientName(stream.readString(40));
			meta.setEnergy(stream.readInt());
			meta.setIntensity(stream.readInt());
			stream.seek(508);
			meta.setDataOffset(stream.readInt());
			// TODO Get Scanco metadata visible to user
		}
	}

	public static class Reader extends ByteArrayReader<Metadata> {

		@Override
		protected String[] createDomainArray() {
			return new String[] { FormatTools.MEDICAL_DOMAIN };
		}

		@Override
		public ByteArrayPlane openPlane(final int imageIndex, final long planeIndex,
			final ByteArrayPlane plane, final long[] planeMin, final long[] planeMax,
			final SCIFIOConfig config) throws FormatException, IOException
		{
			final RandomAccessInputStream stream = getStream();
			final Metadata metadata = getMetadata();
			final int offset = (int) (metadata.dataOffset + metadata.sliceBytes * planeIndex);
			stream.seek(offset);
			return readPlane(stream, imageIndex, planeMin, planeMax, plane);
		}
	}
}
