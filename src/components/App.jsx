import React, { useState } from "react";
import Clock from "./Clock";
import Alarm from "./Alarm";

function App() {

  const [alarmList, setAlarmlist] = useState([]);

  function updateList(minute, hour) {
    setAlarmlist( (prev) => {
      return [...prev, {minute: minute, hour: hour}]
    });
  }

  function deleteAlarm(id) {
    setAlarmlist((prevContent) => {
      return prevContent.filter((item, index) => {
        return index !== id;
      });
    });
  }

  return (
    <div className="app">
      <Clock updateList={updateList} />
      <div className="alarm__container">
        { alarmList && alarmList.map( (item, index) =>
          <Alarm
            minute={item.minute}
            hour={item.hour}
            key={index}
            id={index}
            deleteAlarm={deleteAlarm}
          />
        )}
      </div>
    </div>
  );
}

export default App;
