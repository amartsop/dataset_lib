
    // Estimate velocity (see if velocity belongs to the depedent variables)
    std::string vel_x_str = m_meas_ans[static_cast<int>(meas_index::vel_x)];
    int16_t vel_x_dep = 0;

    for(int i = 0; i < m_file_num; i++)
    {
        if(!vel_x_str.compare(m_meas_dep_vars[0][i])) { vel_x_dep |= (1 << i);}
    }

    if(!(m_meas_ind_vars[0][0].compare(time_str)) && (vel_x_dep == 0) 
        && m_const_vel_x)
    {

        m_velocity = central_diff_derivative(m_time, m_displacement);
    }