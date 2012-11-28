public static class WfsUdpConstants
{
	// commands
	public static int ADD_SOURCE = 1;
	public static int DELETE_SOURCE = 2;
	public static int EDIT_SOURCE = 3;
	public static int EDIT_LOUDSPEAKER = 4;
	public static int EDIT_LINK = 5;
	public static int WHAT_DEVICES_REQUESTED = 6;
	public static int WHAT_DEVICES_RETURNED = 7;
	public static int GET_LOUDSPEAKER_COUNT_REQUESTED = 8;
	public static int GET_LOUDSPEAKER_COUNT_RETURNED = 9;
	public static int ALL_REQUESTED = 10;
	public static int ALL_RETURNED = 11;
	public static int SOURCE_INFO_RETURNED = 12;
	public static int LOUDSPEAKER_INFO_RETURNED = 13;
	public static int SOURCE_COUNT_RETURNED = 14;
	public static int LOUDSPEAKER_COUNT_RETURNED = 15;
	public static int EDIT_SOURCE_POSITION = 16;
	public static int EDIT_SOURCE_AMPLITUDE = 17;
	public static int EDIT_SOURCE_ON= 18;
	public static int SOURCE_REWIND = 19;
	public static int SET_LOCAL_DIRECTORY = 20;
	public static int ADD_SOURCE_DIRECTORY_NOT_SPECIFIED = 21;
	public static int SAY_HELLO = 99;
	public static int LOCAL_AUDIO = 101;
	public static int UDP_RENDERER = 102;
	public static int UDP_CONTROLLER = 103;
}

[Serializable]
public class LoudspeakerInfoPacket
{
	public bool on;
	public double x, y, z, nx, ny, nz;
	public int device, electedOut, channel;
	public LoudspeakerInfoPacket(bool ON, double X, double Y, double Z, double NX, double NY, double NZ, int Device, int ElectedOut, int Channel)
	{
		on = ON;
		x = X;
		y = Y;
		z = Z;
		nx = NX;
		ny = NY;
		nz = NZ;
		electedOut = ElectedOut;
		device = Device;
		channel = Channel;
	}
}

[Serializable]
public class SourceInfoPacket
{
	public bool on, loop;
	public double x, y, z;
	public double a=1.0; // amplitude
	public string wavFile;
	public int renderMethod = 0;
	public SourceInfoPacket(string WavFile, double X, double Y, double Z, bool ON, bool LOOP, double Amplitude)
	{
		on = ON;
		loop = LOOP;
		x = X;
		y = Y;
		z = Z;
		wavFile = WavFile;
		a = Amplitude;
		renderMethod = 0;
	}
	public SourceInfoPacket(string WavFile, double X, double Y, double Z, bool ON, bool LOOP, double Amplitude, int RenderMethod)
	{
		on = ON;
		loop = LOOP;
		x = X;
		y = Y;
		z = Z;
		wavFile = WavFile;
		a = Amplitude;
		renderMethod = RenderMethod;
	}
}

[Serializable]
public class WfsUdpPacket2
{
	public int command = WfsUdpConstants.SAY_HELLO;//1 add source, 2 delete source, 3 edit link
	public int devices=0, sourceCount=0, loudspeakerCount=0;
	//public byte [] packetData =null;
	//public LoudspeakerInfoPacket [] loudspeakerInfo = new LoudspeakerInfoPacket[256];
	//public SourceInfoPacket[] sourceInfo = new SourceInfoPacket[256];
	public LoudspeakerInfo[] loudspeakerInfo = new LoudspeakerInfo[256];
	public SourceInfo[] sourceInfo = new SourceInfo[256];
}

/*
 [Serializable]
 public class WfsUdpPacket2
 {
 public int size = 0;
 public byte [] contents = null;
 public WfsUdpPacket2(int size)
 {
 contents=new byte[size];
 }
 }
 */

public class WfsUdpIO
{
	IPEndPoint ipep = null;
	IPEndPoint ipepIn = null;
	Socket serverIn = null;
	Socket serverOut = null;
	byte[] dataOut = new byte[50000]; // was 1024
	byte[] dataIn = new byte[50000];
	Stream sOut = null;
	Stream sIn = null;
	//WfsUdpPacket[] obj = new WfsUdpPacket[1];
	public WfsUdpPacket2 updPacket = null;
	Thread trd = null;
	DelegateShowReceived showReceivedDelegate = null;
	//public delegate void DelegateShowReceived(WfsUdpPacket[] receivedObj);
	public delegate void DelegateShowReceived(WfsUdpPacket2 receivedObj);
	public string ipDestination = "146.87.112.160";
	public int portListen = 201, portSend = 200;
	BinaryFormatter b = new BinaryFormatter();
	
	public string localWavDirectory = "";
	
	//public byte [] packetData = null;
	
	Form parent = null;
	
	public WfsUdpIO(Form parentApp)
	{
		//obj[0] = new WfsUdpPacket();
		
		updPacket = new WfsUdpPacket2();
		sOut = new MemoryStream(dataOut);
		sIn = new MemoryStream(dataIn);
		parent = parentApp;
	}
	
	public WfsUdpIO()
	{
		//obj[0] = new WfsUdpPacket();
		
		updPacket = new WfsUdpPacket2();
		sOut = new MemoryStream(dataOut);
		sIn = new MemoryStream(dataIn);
		parent = null;
	}
	
	public int setIPDestination(string IP_Destination)
	{
		ipDestination = IP_Destination;
		return 1;
	}
	
	public void setLocalWavDirectory(string LocalWavDirectory)
	{
		localWavDirectory = LocalWavDirectory;
	}
	
	public int sayHello()
	{
		int err;
		updPacket.command = WfsUdpConstants.SAY_HELLO;
		err = sendPacket2();
		return (err);
	}
	
	public int requestAllInfo()
	{
		int err;
		updPacket.command = WfsUdpConstants.ALL_REQUESTED;
		err = sendPacket2();
		return (err);
	}
	
	public int sendAllInfo(ref WFSLibraryM.AudioWrapper audio)
	{
		int i, err = 1;
		byte[] sub = null;
		WFSLibraryM.LoudspeakerInfo loudspeakerInfo = null;
		WFSLibraryM.SourceInfo sourceInfo = null;
		
		updPacket.command = WfsUdpConstants.ALL_RETURNED;
		updPacket.devices = audio.whatDevices();
		updPacket.loudspeakerCount = audio.getLoudspeakerCount();
		updPacket.sourceCount = audio.getSourceCount();
		
		for (i = 0; i < updPacket.loudspeakerCount; i++)
		{
			loudspeakerInfo = null;
			loudspeakerInfo = audio.getLoudspeakerInfo(i);
			updPacket.loudspeakerInfo[i] = null;
			//updPacket.loudspeakerInfo[i] = new LoudspeakerInfoPacket(loudspeakerInfo.on, loudspeakerInfo.x, loudspeakerInfo.y, loudspeakerInfo.z, loudspeakerInfo.nx, loudspeakerInfo.ny, loudspeakerInfo.nz, loudspeakerInfo.deviceIdx,loudspeakerInfo.electedDevice, loudspeakerInfo.channel);
			
			updPacket.loudspeakerInfo[i] = new LoudspeakerInfo();
			updPacket.loudspeakerInfo[i].on = loudspeakerInfo.on;
			updPacket.loudspeakerInfo[i].x = loudspeakerInfo.x;
			updPacket.loudspeakerInfo[i].y = loudspeakerInfo.y;
			updPacket.loudspeakerInfo[i].z = loudspeakerInfo.z;
			updPacket.loudspeakerInfo[i].nx = loudspeakerInfo.nx;
			updPacket.loudspeakerInfo[i].ny = loudspeakerInfo.ny;
			updPacket.loudspeakerInfo[i].nz = loudspeakerInfo.nz;
			updPacket.loudspeakerInfo[i].deviceIdx = loudspeakerInfo.deviceIdx;
			updPacket.loudspeakerInfo[i].electedDevice = loudspeakerInfo.electedDevice;
			updPacket.loudspeakerInfo[i].channel = loudspeakerInfo.channel;
			updPacket.loudspeakerInfo[i].epID = loudspeakerInfo.epID;
			updPacket.loudspeakerInfo[i].loudspeakerIdx = loudspeakerInfo.loudspeakerIdx;
			//  loudspeakerInfo.on, loudspeakerInfo.x, loudspeakerInfo.y, loudspeakerInfo.z, loudspeakerInfo.nx, loudspeakerInfo.ny, loudspeakerInfo.nz, loudspeakerInfo.deviceIdx, loudspeakerInfo.electedDevice, loudspeakerInfo.channel
			
			//sub = null;
			//sub = StructureToByteArray(loudspeakerInfo);
			//updPacket.packetData = ConcatByte(updPacket.packetData, updPacket.packetData.Length, sub, sub.Length);
		}
		
		for (i = 0; i < updPacket.sourceCount; i++)
		{
			sourceInfo = null;
			sourceInfo = audio.getSourceInfo(i);
			updPacket.sourceInfo[i] = null;
			//updPacket.sourceInfo[i] = new SourceInfoPacket(sourceInfo.waveFile, sourceInfo.x, sourceInfo.y, sourceInfo.z, sourceInfo.on ,sourceInfo.loop);
			updPacket.sourceInfo[i] = new SourceInfo();
			updPacket.sourceInfo[i].on = sourceInfo.on;
			updPacket.sourceInfo[i].loop = sourceInfo.loop;
			updPacket.sourceInfo[i].amplitude = sourceInfo.amplitude;
			updPacket.sourceInfo[i].x = sourceInfo.x;
			updPacket.sourceInfo[i].y = sourceInfo.y;
			updPacket.sourceInfo[i].z = sourceInfo.z;
			updPacket.sourceInfo[i].waveFile = sourceInfo.waveFile;
			updPacket.sourceInfo[i].renderMethod = sourceInfo.renderMethod;
			updPacket.sourceInfo[i].sourceIdx = sourceInfo.sourceIdx;
			
			//sub = null;
			//sub = StructureToByteArray(sourceInfo);
			//updPacket.packetData = ConcatByte(updPacket.packetData, updPacket.packetData.Length, sub, sub.Length);
		}
		err = sendPacket2();
		return (err);
	}
	
	public int editRemoteLoudspeaker(int loudspeakerIdx, float x, float y, float z, float nx, float ny, float nz, bool on)
	{
		int err;
		updPacket.command = WfsUdpConstants.EDIT_LOUDSPEAKER;
		updPacket.loudspeakerCount = 1;
		updPacket.loudspeakerInfo[0] = null;
		updPacket.loudspeakerInfo[0] = new LoudspeakerInfo();
		updPacket.loudspeakerInfo[0].loudspeakerIdx = loudspeakerIdx;
		updPacket.loudspeakerInfo[0].x = x;
		updPacket.loudspeakerInfo[0].y = y;
		updPacket.loudspeakerInfo[0].z = z;
		updPacket.loudspeakerInfo[0].nx = nx;
		updPacket.loudspeakerInfo[0].ny = ny;
		updPacket.loudspeakerInfo[0].nz = nz;
		updPacket.loudspeakerInfo[0].on = on;
		err = sendPacket2();
		return (err);
	}
	
	public int editRemoteSource(int sourceIdx, float x, float y, float z, bool on, bool loop, float amplitude, string waveFile)
	{
		int err;
		updPacket.command = WfsUdpConstants.EDIT_SOURCE;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].sourceIdx = sourceIdx;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].on = on;
		updPacket.sourceInfo[0].loop = loop;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].waveFile = waveFile;
		err = sendPacket2();
		return (err);
	}
	
	public int editRemoteSource(int sourceIdx, float x, float y, float z, bool on, bool loop, float amplitude, string waveFile, int renderMethod)
	{
		int err;
		updPacket.command = WfsUdpConstants.EDIT_SOURCE;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].sourceIdx = sourceIdx;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].on = on;
		updPacket.sourceInfo[0].loop = loop;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].waveFile = waveFile;
		updPacket.sourceInfo[0].renderMethod = renderMethod;
		err = sendPacket2();
		return (err);
	}
	
	public int editRemoteSourcePosition(int sourceIdx, float x, float y, float z)
	{
		int err;
		updPacket.command = WfsUdpConstants.EDIT_SOURCE_POSITION;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].sourceIdx = sourceIdx;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].waveFile = "";
		err = sendPacket2();
		return (err);
	}
	
	public int editRemoteSourceAmplitude(int sourceIdx, float amplitude)
	{
		int err;
		updPacket.command = WfsUdpConstants.EDIT_SOURCE_AMPLITUDE;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].sourceIdx = sourceIdx;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].waveFile = "";
		err = sendPacket2();
		return (err);
	}
	
	public int sourceRewind(int sourceIdx)
	{
		int err;
		updPacket.command = WfsUdpConstants.SOURCE_REWIND;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].sourceIdx = sourceIdx;
		updPacket.sourceInfo[0].waveFile = "";
		err = sendPacket2();
		return (err);
	}
	
	public int addRemoteSource(string wavFile, float x, float y, float z, bool on, bool loop, float amplitude)
	{
		int err;
		updPacket.command = WfsUdpConstants.ADD_SOURCE;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].waveFile = wavFile;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].on= on;
		updPacket.sourceInfo[0].loop = loop;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].renderMethod = 0;
		err = sendPacket2();
		return (err);
	}
	
	public int addRemoteSource(string wavFile, float x, float y, float z, bool on, bool loop, float amplitude, int renderMethod)
	{
		int err;
		updPacket.command = WfsUdpConstants.ADD_SOURCE;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].waveFile = wavFile;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].on = on;
		updPacket.sourceInfo[0].loop = loop;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].renderMethod = renderMethod;
		err = sendPacket2();
		return (err);
	}
	
	public int addRemoteSourceGiveWaveName(string wavFileName, float x, float y, float z, bool on, bool loop, float amplitude)
	{
		int err;
		updPacket.command = WfsUdpConstants.ADD_SOURCE_DIRECTORY_NOT_SPECIFIED;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].waveFile = wavFileName;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].on = on;
		updPacket.sourceInfo[0].loop = loop;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].renderMethod = 0;
		err = sendPacket2();
		return (err);
	}
	
	public int addRemoteSourceGiveWaveName(string wavFileName, float x, float y, float z, bool on, bool loop, float amplitude, int renderMethod)
	{
		int err;
		updPacket.command = WfsUdpConstants.ADD_SOURCE_DIRECTORY_NOT_SPECIFIED;
		updPacket.sourceCount = 1;
		updPacket.sourceInfo[0] = null;
		updPacket.sourceInfo[0] = new SourceInfo();
		updPacket.sourceInfo[0].waveFile = wavFileName;
		updPacket.sourceInfo[0].x = x;
		updPacket.sourceInfo[0].y = y;
		updPacket.sourceInfo[0].z = z;
		updPacket.sourceInfo[0].on = on;
		updPacket.sourceInfo[0].loop = loop;
		updPacket.sourceInfo[0].amplitude = amplitude;
		updPacket.sourceInfo[0].renderMethod = renderMethod;
		err = sendPacket2();
		return (err);
	}
	
	public string errMessage = "";
	
	public int sendPacket2()
	{
		int len;
		ipep = null;
		serverOut = null;
		
		ipep = new IPEndPoint(IPAddress.Parse(ipDestination), portSend);
		serverOut = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
		
		len = writeInfo();
		try
		{
			serverOut.SendTo(dataOut, len, SocketFlags.None, ipep);
			serverOut.Close();
		}
		catch 
		{
			return (-1);
		}
		return 1;
	}
	
	public int writeInfo()
	{
		int i, length=0;
		sOut.Position = 0;
		BinaryWriter bw = new BinaryWriter(sOut);
		bw.Write(updPacket.command);
		bw.Write(updPacket.devices);
		bw.Write(updPacket.loudspeakerCount);
		bw.Write(updPacket.sourceCount);
		
		for (i = 0; i < updPacket.loudspeakerCount; i++)
		{ 
			bw.Write(updPacket.loudspeakerInfo[i].deviceIdx);
			bw.Write(updPacket.loudspeakerInfo[i].electedDevice);
			bw.Write(updPacket.loudspeakerInfo[i].channel);
			bw.Write(updPacket.loudspeakerInfo[i].loudspeakerIdx);
			bw.Write(updPacket.loudspeakerInfo[i].on);
			bw.Write(updPacket.loudspeakerInfo[i].x);
			bw.Write(updPacket.loudspeakerInfo[i].y);
			bw.Write(updPacket.loudspeakerInfo[i].z);
			bw.Write(updPacket.loudspeakerInfo[i].nx);
			bw.Write(updPacket.loudspeakerInfo[i].ny);
			bw.Write(updPacket.loudspeakerInfo[i].nz);
			bw.Write(updPacket.loudspeakerInfo[i].epID);
		}
		for (i = 0; i < updPacket.sourceCount; i++)
		{
			bw.Write(updPacket.sourceInfo[i].sourceIdx);
			bw.Write(updPacket.sourceInfo[i].amplitude);
			bw.Write(updPacket.sourceInfo[i].on);
			bw.Write(updPacket.sourceInfo[i].loop);
			bw.Write(updPacket.sourceInfo[i].x);
			bw.Write(updPacket.sourceInfo[i].y);
			bw.Write(updPacket.sourceInfo[i].z);
			bw.Write(updPacket.sourceInfo[i].renderMethod);
			bw.Write(updPacket.sourceInfo[i].waveFile);
		}
		length = (int)sOut.Position;
		return (length);
	}
	
	public int extractInfo()
	{
		int i, length = 0;
		sIn.Position = 0;
		BinaryReader br = new BinaryReader(sIn);
		updPacket.command = br.ReadInt32();
		updPacket.devices = br.ReadInt32();
		updPacket.loudspeakerCount = br.ReadInt32();
		updPacket.sourceCount = br.ReadInt32();
		
		for (i = 0; i < updPacket.loudspeakerCount; i++)
		{
			updPacket.loudspeakerInfo[i] = null;
			updPacket.loudspeakerInfo[i] = new LoudspeakerInfo();
			updPacket.loudspeakerInfo[i].deviceIdx = br.ReadInt32();
			updPacket.loudspeakerInfo[i].electedDevice = br.ReadInt32();
			updPacket.loudspeakerInfo[i].channel = br.ReadInt32();
			updPacket.loudspeakerInfo[i].loudspeakerIdx = br.ReadInt32();
			updPacket.loudspeakerInfo[i].on = br.ReadBoolean();
			updPacket.loudspeakerInfo[i].x = br.ReadSingle();
			updPacket.loudspeakerInfo[i].y = br.ReadSingle();
			updPacket.loudspeakerInfo[i].z = br.ReadSingle();
			updPacket.loudspeakerInfo[i].nx = br.ReadSingle();
			updPacket.loudspeakerInfo[i].ny = br.ReadSingle();
			updPacket.loudspeakerInfo[i].nz = br.ReadSingle();
			updPacket.loudspeakerInfo[i].epID = br.ReadString();
		}
		for (i = 0; i < updPacket.sourceCount; i++)
		{
			updPacket.sourceInfo[i] = null;
			updPacket.sourceInfo[i] = new SourceInfo();
			updPacket.sourceInfo[i].sourceIdx = br.ReadInt32();
			updPacket.sourceInfo[i].amplitude = br.ReadSingle();
			updPacket.sourceInfo[i].on = br.ReadBoolean();
			updPacket.sourceInfo[i].loop = br.ReadBoolean();
			updPacket.sourceInfo[i].x = br.ReadSingle();
			updPacket.sourceInfo[i].y = br.ReadSingle();
			updPacket.sourceInfo[i].z = br.ReadSingle();
			updPacket.sourceInfo[i].renderMethod = br.ReadInt32();
			updPacket.sourceInfo[i].waveFile = br.ReadString();
		}
		length = (int)sIn.Position;
		return (length);
	}
	
	public void setSendAndReceivePort(int portToSendToo, int portToListenOn)
	{
		portListen = portToListenOn;
		portSend = portToSendToo;
	}
	
	public void startListeningForPackets(DelegateShowReceived showReceivedDelegateCallback)
	{
		trd = new Thread(new ThreadStart(serverRunning));
		trd.IsBackground = true;
		trd.Start();
		showReceivedDelegate = showReceivedDelegateCallback;
	}
	
	public void serverRunning()
	{
		Stream s = null;
		serverIn = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
		ipepIn = new IPEndPoint(IPAddress.Any, portListen);
		EndPoint ep = (EndPoint)ipepIn;
		serverIn.Bind(ep);
		
		while (true)
		{
			try
			{
				serverIn.ReceiveFrom(dataIn, ref ep);
				
				sIn = null;
				sIn = new MemoryStream(dataIn);
				
			}
			catch
			{
				trd.Abort();
			}
			
			try
			{
				extractInfo();
			}
			catch
			{
				updPacket.command = -1;
				updPacket.devices = 0;
				updPacket.loudspeakerCount = 0;
				updPacket.sourceCount= 0;
			}
			
			try
			{
				parent.Invoke(showReceivedDelegate, updPacket);
			}
			catch
			{
				trd.Abort();
			}
		}
	}
	
	public void endListeningForPackets()
	{
		trd.Abort();
		serverIn.Close();
	}
	
	// might be useful
	static byte[] StructureToByteArray(object obj)
	{
		int len = Marshal.SizeOf(obj);
		
		byte[] arr = new byte[len];
		IntPtr ptr = Marshal.AllocHGlobal(len);
		Marshal.StructureToPtr(obj, ptr, true);
		Marshal.Copy(ptr, arr, 0, len);
		Marshal.FreeHGlobal(ptr);
		return arr;
	}
	
	static void ByteArrayToStructure(byte[] bytearray, ref object obj)
	{
		int len = Marshal.SizeOf(obj);
		
		IntPtr i = Marshal.AllocHGlobal(len);
		Marshal.Copy(bytearray, 0, i, len);
		obj = Marshal.PtrToStructure(i, obj.GetType());
		Marshal.FreeHGlobal(i);
	}
	
	public byte[] ConcatByte(byte[] a, int aSize, byte[] b, int bSize)
	{
		byte[] buff = new byte[aSize + bSize];
		for (int i = 0; i < aSize; i++)
			buff[i] = a[i];
		for (int i = 0; i < bSize; i++)
			buff[i + aSize] = b[i];
		return buff;
	}
	
}