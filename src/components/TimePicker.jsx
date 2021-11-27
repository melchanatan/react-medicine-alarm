import React, { useEffect, useState } from "react";

function TimePicker(props) {

  const [value, setValue] = useState(0);
  const maxValue = props.maxValue;
  const increment = props.increment;
  
  const formatNumber = myNumber => ("0" + myNumber).slice(-2);

  useEffect(() => {
    props.passValue(value, props.id);
  })

  function timeUp() {
    if (value + increment < maxValue) setValue( prev => prev + increment);
    else setValue(0);
  }

  function timeDown() {
    if (value - increment >= 0) setValue(prev => prev - increment);
    else setValue(maxValue - increment);
  }

  function handleChange(event) {
    const targetValue = parseInt(event.target.value);
    if (targetValue >= maxValue || targetValue < 0) setValue(0);
    else setValue(targetValue);
  }


  return (
    <div className="clock__container">
        <button className="btn clock-btn" onClick={timeUp} ><i className="fas fa-chevron-up"></i></button>
        <input type="number" className="clock__label" min="0" max="60" onChange={handleChange} value={formatNumber(value)}/>
        <button className="btn clock-btn" onClick={timeDown}><i className="fas fa-chevron-down"></i></button>
      </div>
  );
}

export default TimePicker;
