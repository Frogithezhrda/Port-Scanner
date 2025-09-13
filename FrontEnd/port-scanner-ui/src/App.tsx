import { useState, useEffect } from "react";
import PortList from "./components/PortList";
import ProgressBar from "./components/ProgressBar";
import "./App.css";

interface ScanData {
  port?: number;
  scanned?: number;
  total?: number;
}

function App() {
  const [targetIP, setTargetIP] = useState("");
  const [startPort, setStartPort] = useState(21);
  const [endPort, setEndPort] = useState(80);
  const [numberOfThreads, setThreadCount] = useState(4);
  const [openPorts, setOpenPorts] = useState<number[]>([]);
  const [scannedPorts, setScannedPorts] = useState(0);
  const [totalPorts, setTotalPorts] = useState(0);
  const [isScanning, setIsScanning] = useState(false);
  const [ws, setWs] = useState<WebSocket | null>(null);

  useEffect(() => {
    const socket = new WebSocket("ws://localhost:8080");
    socket.onmessage = (event) => {
      const data: ScanData & { finished?: boolean } = JSON.parse(event.data);
    
      if (data.port) setOpenPorts(prev => [...prev, data.port as number]);
      if (data.scanned) setScannedPorts(data.scanned);
      if (data.total) setTotalPorts(data.total);
    
      if (data.finished) {
        setIsScanning(false); 
        console.log("Scan complete!");
      }
    };
    
    setWs(socket);
    return () => socket.close();
  }, []);

  const startScan = () => {
    if (!ws) return;

    if (!targetIP.trim()) {
      alert("Please enter a Target IP.");
      return;
    }
    if (!startPort || !endPort || startPort < 0 || endPort < 0) {
      alert("Please enter valid start and end ports.");
      return;
    }
    if (startPort > endPort) {
      alert("Start port cannot be greater than end port.");
      return;
    }
    if (!numberOfThreads || numberOfThreads < 1) {
      alert("Please enter a valid number of threads.");
      return;
    }   
    setOpenPorts([]);
    setScannedPorts(0);
    setTotalPorts(endPort - startPort + 1);
    setIsScanning(true);
    ws.send(JSON.stringify({ numberOfThreads, targetIP, startPort, endPort }));
  };

  return (
    <div className="app-container">
      <h1>Port Scanner</h1>

      <div className="input-row">
        <input
          type="text"
          placeholder="Target IP"
          value={targetIP}
          onChange={(e) => setTargetIP(e.target.value)}
          disabled={isScanning}
        />
        <input
          type="number"
          placeholder="Start Port"
          value={startPort}
          onChange={(e) => setStartPort(Number(e.target.value))}
          disabled={isScanning}
        />
        <input
          type="number"
          placeholder="End Port"
          value={endPort}
          onChange={(e) => setEndPort(Number(e.target.value))}
          disabled={isScanning}
        />
        <input
          type="number"
          placeholder="Thread Count"
          value={numberOfThreads}
          onChange={(e) => setThreadCount(Number(e.target.value))}
          disabled={isScanning}
        />
        <button onClick={startScan} disabled={isScanning}>
          {isScanning ? "Scanning..." : "Scan"}
        </button>
      </div>

      <ProgressBar scanned={scannedPorts} total={totalPorts} />

      <h2>Open Ports:</h2>
      <PortList ports={openPorts} />
    </div>
  );
}

export default App;
