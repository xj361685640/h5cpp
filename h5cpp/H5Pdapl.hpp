/*
 * Copyright (c) 2018 vargaconsulting, Toronto,ON Canada
 * Author: Varga, Steven <steven@vargaconsulting.ca>
 *
 */

#ifndef  H5CPP_PDAPL_HPP
#define  H5CPP_PDAPL_HPP

#define H5CPP_DAPL_HIGH_THROUGPUT "h5cpp_dapl_highthroughput"

namespace h5 { namespace impl {
	inline herr_t dapl_pipeline_create( const char *name, size_t size, void *initial_value){
		return 0;
	}
	inline herr_t dapl_pipeline_set( ::hid_t dapl_id, const char *name, size_t size, void *new_value){
		return 0;
	}
	inline herr_t dapl_pipeline_get( ::hid_t dapl_id, const char *name, size_t size, void *value){
		return 0;
	}
	inline herr_t dapl_pipeline_delete( ::hid_t dapl_id, const char *name, size_t size, void *value){
		std::cout <<"< dapl delete:" << dapl_id <<">\n";
		return 0;
	}
	inline herr_t dapl_pipeline_copy( const char *name, size_t size, void *value){
		return 0;
	}
	inline int dapl_pipeline_comp( const void *value1, const void *value2, size_t size){
		return 0;
	}

	inline herr_t dapl_pipeline_close( const char *name, size_t size, void *ptr ){
 		std::cout <<"<dapl delete: " << *static_cast< impl::pipeline_t<impl::basic_pipeline_t>**>( ptr ) <<">\n";
		delete *static_cast< impl::pipeline_t<impl::basic_pipeline_t>**>( ptr );
		return 0;
	}

	inline ::herr_t dapl_pipeline_set(::hid_t dapl ){
		// ignore if already set 
		if( H5Pexist(dapl, H5CPP_DAPL_HIGH_THROUGPUT) ) return 0;
		using pipeline = impl::pipeline_t<impl::basic_pipeline_t>;
	 	pipeline* ptr = new pipeline();
		std::cout <<"<dapl create: " << ptr <<">\n";
		return H5Pinsert2(dapl, H5CPP_DAPL_HIGH_THROUGPUT, sizeof( pipeline* ), &ptr,
				dapl_pipeline_set, dapl_pipeline_get, dapl_pipeline_delete, dapl_pipeline_copy, dapl_pipeline_comp, dapl_pipeline_close);
	}
}}

namespace h5 {
// DATA ACCESS PROPERTY LISTS
	using chunk_cache          = impl::dapl_call< impl::dapl_args<hid_t,size_t, size_t, double>,H5Pset_chunk_cache>;
	using efile_prefix 		   = impl::dapl_call< impl::dapl_args<hid_t,const char*>,H5Pset_efile_prefix>;
	using virtual_view         = impl::dapl_call< impl::dapl_args<hid_t,H5D_vds_view_t>,H5Pset_virtual_view>;
	using virtual_printf_gap   = impl::dapl_call< impl::dapl_args<hid_t,hsize_t>,H5Pset_virtual_printf_gap>;
	//using num_threads  	   	   = impl::dapl_call< impl::dapl_args<hid_t, unsigned char>,impl::dapl_threads>;
	namespace flag {
		using high_throughput      = impl::dapl_call< impl::dapl_args<hid_t>,impl::dapl_pipeline_set>;
	}
	const static flag::high_throughput high_throughput;

	const static h5::dapl_t dapl = static_cast<h5::dapl_t>( H5Pcreate(H5P_DATASET_ACCESS) );
	//const static h5::dapl_t default_dapl = high_throughput;
	const static h5::dapl_t default_dapl = static_cast<h5::dapl_t>(  H5Pcreate(H5P_DATASET_ACCESS) );
}

#endif
