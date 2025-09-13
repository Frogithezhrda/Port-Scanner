import React from "react";

interface ProgressBarProps {
  scanned: number;
  total: number;
}

const ProgressBar: React.FC<ProgressBarProps> = ({ scanned, total }) => {
  const percent = total === 0 ? 0 : (scanned / total) * 100;
  return (
    <div className="progress-bar">
      <div className="progress-fill" style={{ width: `${percent}%` }}></div>
    </div>
  );
};

export default ProgressBar;
