import React from 'react'
import { BrowserRouter}  from 'react-router-dom'
import { Route } from 'react-router-dom'
import RegForm from './components/RegForm'
import Table from './components/Table'

const routes  = () => {
    return(
        <BrowserRouter>
            <Route exact path="/form" component={RegForm}></Route>
            <Route exact path="/" component={Table}></Route>
        </BrowserRouter>
    )
}

export default routes

