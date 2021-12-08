import React from "react";
import TimePicker from "./TimePicker"


const currentTime = {minute: 0, hour: 0};
function Clock(props) {

  function passValue(value, id) {
    currentTime[id] = value;
  }

  function setAlarm() {
    props.updateList(currentTime.minute, currentTime.hour);
  }

  return (
    <div className="clock-container">
    <h3>Set your Medicine Alarm</h3>
      <div className="clock">
          <TimePicker maxValue={24} increment={1} passValue={passValue} id="hour" key="hour" />
          <span className="clock-colon">:</span>
          <TimePicker maxValue={60} increment={5} passValue={passValue} id="minute" key="minute" />
          <button className="clock__set-alarm" onClick={setAlarm}><i className="fal fa-alarm-plus"></i></button>
      </div>
    </div>
  );
}

export default Clock;
