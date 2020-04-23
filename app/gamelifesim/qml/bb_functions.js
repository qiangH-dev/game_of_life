.import net.phoneyou.COVID19.BB 1.0 as BB

function changeContactDegree(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeContactDegree],contact_degree:value})
}
function changeSisModel(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeSISModel],sis_model:value})
}
function changeSyncDurationTime(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeSyncDurationTime],sync_duration_time:value})
}
function changeTreatmentDuration(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeTreatmentDuration],duration:{treatment:value}})
}
function changeInfectDuration(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeInfectDuration],duration:{infect:value}})
}
function changeIncubationInfectDuration(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeIncubationInfectDuration],duration:{incubation_infect:value}})
}
function changeIncubationDuration(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeIncubationDuration],duration:{incubation:value}})
}
function changeSpontaneousActivationDuration(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeSpontaneousActivationDuration],duration:{spontaneous_activation:value}})
}
function changeTransmissionRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeTransmissionRate],rate:{transmission:value / 100.0}})
}
function changeRecoverRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeRecoverRate],rate:{recover:value / 100.0}})
}
function changeImmunityRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeImmunityRate],rate:{immunity:value / 100.0}})
}
function changeDeathRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeDeathRate],rate:{death:value / 100.0}})
}
function changeIncubationTransmissionRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeIncubationTransmissionRate],rate:{incubation_transmission:value / 100.0}})
}
function changeSpontaneousActivationRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeSpontaneousActivationRate],rate:{spontaneous_activation:value / 100.0}})
}
function changeIncubationInfectRate(value){
    return __changeModelSettings({options:[BB.UpdateLemologyModel.ChangeIncubationInfectedRate],rate:{incubation_infected:value / 100.0}})
}

function clearCells(){
    BB.Notification.rpc_reset_all_cells({type:BB.GridType.Susceptible})
}

function changeCellType(id,type,radius){
    BB.Notification.rpc_change_cell_status({met_id : id , type : type , radius:radius})
}

function changeCellRecovered(id,type,radius){
    BB.Notification.rpc_change_cell_status({met_id : id , type : BB.GridType.Recovered , recovered_type:type , radius:radius})
}

function generateCity(id,radius,minDegree,maxDegree){
    BB.Notification.rpc_generate_a_city({center_id: id , radius: radius ,min_degree : minDegree , max_degree : maxDegree})
}

function __changeModelSettings(msg){
    BB.Notification.rpc_update_lemology_model(msg)
    return true
}
