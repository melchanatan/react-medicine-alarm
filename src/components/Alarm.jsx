import React from "react";

function Alarm(props) {

  const formatNumber = myNumber => ("0" + myNumber).slice(-2);
  return (
    <div className="alarm">
      <span className="alarm__label">{formatNumber(props.hour)}</span>
      <span className="clock-colon">:</span>
      <span className="alarm__label">{formatNumber(props.minute)}</span>
      <button className="alarm-btn" onClick={ () => props.deleteAlarm(props.id)}><i className="fas fa-trash"></i></button>
    </div>
  );
}

export default Alarm;
