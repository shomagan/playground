typedef struct nic_stats {
  /*tx counters*/
  uint64_t tx_octetcount_gb;
  uint32_t tx_framecount_gb;
  uint32_t tx_broadcastframe_g;
  uint32_t tx_multicastframe_g;
  uint64_t tx_64_octets_gb;
  uint64_t tx_65_to_127_octets_gb;
  uint64_t tx_128_to_255_octets_gb;
  uint64_t tx_256_to_511_octets_gb;
  uint64_t tx_512_to_1023_octets_gb;
  uint64_t tx_1024_to_max_octets_gb;
  uint32_t tx_unicast_gb;
  uint32_t tx_multicast_gb;
  uint32_t tx_broadcast_gb;
  uint32_t tx_underflow_error;
  uint32_t tx_singlecol_g;
  uint32_t tx_multicol_g;
  uint32_t tx_deferred;
  uint32_t tx_latecol;
  uint32_t tx_exesscol;
  uint32_t tx_errors;
  uint32_t tx_fifo_error;
  uint32_t tx_carrier_error;
  uint32_t tx_aborted_errors;
  uint32_t tx_window_errors;
  uint64_t tx_octetcount_g;
  uint32_t tx_framecount_g;
  uint32_t tx_excessdef;
  uint32_t tx_pause_frame;
  uint32_t tx_osize_frame_g;

/*rx counters*/
  uint32_t rx_framecount_gb;
  uint64_t rx_octetcount_gb;
  uint64_t rx_octetcount_g;
  uint32_t rx_broadcastframe_g;
  uint32_t rx_multicastframe_g;
  uint32_t rx_errors;
  uint32_t rx_crc_error;
  uint32_t rx_frame_error;
  uint32_t rx_fifo_error;
  uint32_t rx_align_error;
  uint32_t rx_run_error;
  uint32_t rx_jabber_error;
  uint32_t rx_undersize_g;
  uint32_t rx_oversize_g;
  uint64_t rx_64_octets_gb;
  uint64_t rx_65_to_127_octets_gb;
  uint64_t rx_128_to_255_octets_gb;
  uint64_t rx_256_to_511_octets_gb;
  uint64_t rx_512_to_1023_octets_gb;
  uint64_t rx_1024_to_max_octets_gb;
  uint32_t rx_unicast_g;
  uint32_t rx_length_error;
  uint32_t rx_outofrangetype;
  uint32_t rx_pause_frames;
  uint32_t rx_fifo_overflow;
  uint32_t rx_vlan_frames_gb;
  uint32_t rx_watchdog_error;
  uint32_t rx_receive_error;
  uint32_t rx_ctrl_frames_g;

  /* Protocols */
  uint32_t rx_udp_gd;
  uint32_t rx_udp_err;
  uint32_t rx_tcp_gd;
  uint32_t rx_tcp_err;
  uint32_t rx_icmp_gd;
  uint32_t rx_icmp_err;

  /* Protocols */
  uint64_t rx_udp_gd_octets;
  uint64_t rx_udp_err_octets;
  uint64_t rx_tcp_gd_octets;
  uint64_t rx_tcp_err_octets;
  uint64_t rx_icmp_gd_octets;
  uint64_t rx_icmp_err_octets;
}emac_nic_stats_t;

