import React from "react";

interface PortListProps {
  ports: number[];
}

const PortList: React.FC<PortListProps> = ({ ports }) => {
  return (
    <div className="port-list">
      {ports.map((port) => (
        <span key={port} className="port-badge">
          {port}
        </span>
      ))}
    </div>
  );
};

export default PortList;
