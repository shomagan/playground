  /*tx counters*/
std::cout << "tx_octetcount_gb " << nic_stat.tx_octetcount_gb << std::endl;
std::cout << "tx_framecount_gb " << nic_stat.tx_framecount_gb << std::endl;
std::cout << "tx_broadcastframe_g " << nic_stat.tx_broadcastframe_g << std::endl;
std::cout << "tx_multicastframe_g " << nic_stat.tx_multicastframe_g << std::endl;
std::cout << "tx_64_octets_gb " << nic_stat.tx_64_octets_gb << std::endl;
std::cout << "tx_65_to_127_octets_gb " << nic_stat.tx_65_to_127_octets_gb << std::endl;
std::cout << "tx_128_to_255_octets_gb " << nic_stat.tx_128_to_255_octets_gb << std::endl;
std::cout << "tx_256_to_511_octets_gb " << nic_stat.tx_256_to_511_octets_gb << std::endl;
std::cout << "tx_512_to_1023_octets_gb " << nic_stat.tx_512_to_1023_octets_gb << std::endl;
std::cout << "tx_1024_to_max_octets_gb " << nic_stat.tx_1024_to_max_octets_gb << std::endl;
std::cout << "tx_unicast_gb " << nic_stat.tx_unicast_gb << std::endl;
std::cout << "tx_multicast_gb " << nic_stat.tx_multicast_gb << std::endl;
std::cout << "tx_broadcast_gb " << nic_stat.tx_broadcast_gb << std::endl;
std::cout << "tx_underflow_error " << nic_stat.tx_underflow_error << std::endl;
std::cout << "tx_singlecol_g " << nic_stat.tx_singlecol_g << std::endl;
std::cout << "tx_multicol_g " << nic_stat.tx_multicol_g << std::endl;
std::cout << "tx_deferred " << nic_stat.tx_deferred << std::endl;
std::cout << "tx_latecol " << nic_stat.tx_latecol << std::endl;
std::cout << "tx_exesscol " << nic_stat.tx_exesscol << std::endl;
std::cout << "tx_errors " << nic_stat.tx_errors << std::endl;
std::cout << "tx_fifo_error " << nic_stat.tx_fifo_error << std::endl;
std::cout << "tx_carrier_error " << nic_stat.tx_carrier_error << std::endl;
std::cout << "tx_aborted_errors " << nic_stat.tx_aborted_errors << std::endl;
std::cout << "tx_window_errors " << nic_stat.tx_window_errors << std::endl;
std::cout << "tx_octetcount_g " << nic_stat.tx_octetcount_g << std::endl;
std::cout << "tx_framecount_g " << nic_stat.tx_framecount_g << std::endl;
std::cout << "tx_excessdef " << nic_stat.tx_excessdef << std::endl;
std::cout << "tx_pause_frame " << nic_stat.tx_pause_frame << std::endl;
std::cout << "tx_osize_frame_g " << nic_stat.tx_osize_frame_g << std::endl;

/*rx counters*/
std::cout << "rx_framecount_gb " << nic_stat.rx_framecount_gb << std::endl;
std::cout << "rx_octetcount_gb " << nic_stat.rx_octetcount_gb << std::endl;
std::cout << "rx_octetcount_g " << nic_stat.rx_octetcount_g << std::endl;
std::cout << "rx_broadcastframe_g " << nic_stat.rx_broadcastframe_g << std::endl;
std::cout << "rx_multicastframe_g " << nic_stat.rx_multicastframe_g << std::endl;
std::cout << "rx_errors " << nic_stat.rx_errors << std::endl;
std::cout << "rx_crc_error " << nic_stat.rx_crc_error << std::endl;
std::cout << "rx_frame_error " << nic_stat.rx_frame_error << std::endl;
std::cout << "rx_fifo_error " << nic_stat.rx_fifo_error << std::endl;
std::cout << "rx_align_error " << nic_stat.rx_align_error << std::endl;
std::cout << "rx_run_error " << nic_stat.rx_run_error << std::endl;
std::cout << "rx_jabber_error " << nic_stat.rx_jabber_error << std::endl;
std::cout << "rx_undersize_g " << nic_stat.rx_undersize_g << std::endl;
std::cout << "rx_oversize_g " << nic_stat.rx_oversize_g << std::endl;
std::cout << "rx_64_octets_gb " << nic_stat.rx_64_octets_gb << std::endl;
std::cout << "rx_65_to_127_octets_gb " << nic_stat.rx_65_to_127_octets_gb << std::endl;
std::cout << "rx_128_to_255_octets_gb " << nic_stat.rx_128_to_255_octets_gb << std::endl;
std::cout << "rx_256_to_511_octets_gb " << nic_stat.rx_256_to_511_octets_gb << std::endl;
std::cout << "rx_512_to_1023_octets_gb " << nic_stat.rx_512_to_1023_octets_gb << std::endl;
std::cout << "rx_1024_to_max_octets_gb " << nic_stat.rx_1024_to_max_octets_gb << std::endl;
std::cout << "rx_unicast_g " << nic_stat.rx_unicast_g << std::endl;
std::cout << "rx_length_error " << nic_stat.rx_length_error << std::endl;
std::cout << "rx_outofrangetype " << nic_stat.rx_outofrangetype << std::endl;
std::cout << "rx_pause_frames " << nic_stat.rx_pause_frames << std::endl;
std::cout << "rx_fifo_overflow " << nic_stat.rx_fifo_overflow << std::endl;
std::cout << "rx_vlan_frames_gb " << nic_stat.rx_vlan_frames_gb << std::endl;
std::cout << "rx_watchdog_error " << nic_stat.rx_watchdog_error << std::endl;
std::cout << "rx_receive_error " << nic_stat.rx_receive_error << std::endl;
std::cout << "rx_ctrl_frames_g " << nic_stat.rx_ctrl_frames_g << std::endl;

  /* Protocols */
std::cout << "rx_udp_gd " << nic_stat.rx_udp_gd << std::endl;
std::cout << "rx_udp_err " << nic_stat.rx_udp_err << std::endl;
std::cout << "rx_tcp_gd " << nic_stat.rx_tcp_gd << std::endl;
std::cout << "rx_tcp_err " << nic_stat.rx_tcp_err << std::endl;
std::cout << "rx_icmp_gd " << nic_stat.rx_icmp_gd << std::endl;
std::cout << "rx_icmp_err " << nic_stat.rx_icmp_err << std::endl;

  /* Protocols */
std::cout << "rx_udp_gd_octets " << nic_stat.rx_udp_gd_octets << std::endl;
std::cout << "rx_udp_err_octets " << nic_stat.rx_udp_err_octets << std::endl;
std::cout << "rx_tcp_gd_octets " << nic_stat.rx_tcp_gd_octets << std::endl;
std::cout << "rx_tcp_err_octets " << nic_stat.rx_tcp_err_octets << std::endl;
std::cout << "rx_icmp_gd_octets " << nic_stat.rx_icmp_gd_octets << std::endl;
std::cout << "rx_icmp_err_octets " << nic_stat.rx_icmp_err_octets << std::endl;
}emac_nic_stats_t;

