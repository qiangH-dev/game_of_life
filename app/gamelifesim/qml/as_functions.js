.import net.phoneyou.AMSTER.BB 1.0 as BB
.import net.phoneyou.AMSTER 1.0 as AS
// rate
function fastForward(){
    var _next = __nextSpeed(true)
    __changeSpeed(_next)
}
function fastBackward(){
    var _next = __nextSpeed(false)
    __changeSpeed(_next)
}
function fastReset(){
    __changeSpeed(BB.Rate.X1)
}


function saveSnapshot(url){
    AS.BBAccessor.snapshot(__urlToFilePath(url),true,false)
}

function restoreSnapshot(url){
    AS.BBAccessor.snapshot(__urlToFilePath(url),true,true)
}

function __urlToFilePath(url){
    var path = url.toString();
    // remove prefixed "file:///"
    path = path.replace(/^(file:\/{3})/,"");
    // unescape html codes like '%23' for '#'
    return decodeURIComponent(path);
}
// status
function start()
{
    __changeStatus(BB.EngineControl.Start)
}
function stop()
{
    __changeStatus(BB.EngineControl.Stop)
}
function suspend()
{
    __changeStatus(BB.EngineControl.Suspend)
}

function setRatedStep(rated_step_msec) {
    __changeRatedStep(rated_step_msec)
}

// gbbtime
function jumpToTimePoint(jump_to_time_msec){
    __jumpToTimePoint(jump_to_time_msec)
}
function jumpTimeOffset(offset_msec){
    __jumpTimeOffset(offset_msec)
}

// tick mode
function changeTickModeToSimulation(){
    __changeTickMode(BB.TickMode.Simulation)
}
function changeTickModeToReplaying(){
    __changeTickMode(BB.TickMode.Replaying)
}
function changeTickModeToCommon(){
    __changeTickMode(BB.TickMode.Common)
}
function changeTickModeToDirectEditing(){
    __changeTickMode(BB.TickMode.DirectEditing)
}

function fastRateText(rate){

    switch(rate){
    case BB.Rate.X_25 :
        return "1/4"
    case BB.Rate.X_5 :
        return "1/2"
    case BB.Rate.X1 :
        return "1"
    case BB.Rate.X2 :
        return "2"
    case BB.Rate.X4 :
        return "4"
    case BB.Rate.X8 :
        return "8"
    case BB.Rate.X16 :
        return "16"
    case BB.Rate.X50:
        return "50"
    default:
        return "0"
    }
}

function __nextSpeed(_forward)
{
    var _currentRate = BB.Global.EngineInfo.rate
    if(_currentRate === BB.Rate.InvalidRate){
        return BB.Rate.X1
    }
    if(_forward){
        switch(_currentRate){
        case BB.Rate.X_25 :
            return BB.Rate.X_5
        case BB.Rate.X_5 :
            return BB.Rate.X1
        case BB.Rate.X1 :
            return BB.Rate.X2
        case BB.Rate.X2 :
            return BB.Rate.X4
        case BB.Rate.X4 :
            return BB.Rate.X8
        case BB.Rate.X8 :
            return BB.Rate.X16
        case BB.Rate.X16 :
            return BB.Rate.X50
        default :
            return BB.Rate.X50
        }
    }else{
        switch(_currentRate){
        case BB.Rate.X_5 :
            return BB.Rate.X_25
        case BB.Rate.X1 :
            return BB.Rate.X_5
        case BB.Rate.X2 :
            return BB.Rate.X1
        case BB.Rate.X4 :
            return BB.Rate.X2
        case BB.Rate.X8 :
            return BB.Rate.X4
        case BB.Rate.X16 :
            return BB.Rate.X8
        case BB.Rate.X50 :
            return BB.Rate.X16
        default :
            return BB.Rate.X_25
        }
    }
}
function __changeSpeed(_speed)
{
    BB.Notification.pub_EngineControl(
                {
                    commander:BB.EngineControl.ChangeRate,
                    changed_rate : _speed
                })
}

function __changeStatus(_status){
    BB.Notification.pub_EngineControl(
                {
                    commander : _status
                })
}
function __changeStatus2Running(_status,scenario_begin_time){
    BB.Notification.pub_EngineControl(
                {
                    commander : _status,
                    scenario_begin_time : scenario_begin_time
                })
}
function __changeRatedStep(changed_rated_step_msec){
    BB.Notification.pub_EngineControl(
                {
                    commander : BB.EngineControl.ChangeRatedTick,
                    changed_rated_step_msec : changed_rated_step_msec
                })
}

function __jumpToTimePoint(jump_to_time_msec){
    BB.Notification.pub_EngineControl(
                {
                    commander : BB.EngineControl.TimeJumpTo,
                    jump_to_time_msec : jump_to_time_msec
                })
}
function __jumpTimeOffset(offset_msec){
    BB.Notification.pub_EngineControl(
                {
                    commander : BB.EngineControl.TimeJumpOffset,
                    offset_msec : offset_msec
                })
}

function __changeTickMode(tick_mode)
{
    if(tick_mode === BB.Global.EngineInfo.tick_mode){
        return
    }
    BB.Notification.pub_EngineControl(
                {
                    commander : BB.EngineControl.ChangeTickMode,
                    changed_tick_mode : tick_mode
                })
}
