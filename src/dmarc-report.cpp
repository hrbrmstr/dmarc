#include <Rcpp.h>
#include <opendmarc/dmarc.h>

using namespace Rcpp;

// [[Rcpp::export]]
std::vector< std::string > parse_report_from_file_int(std::string path) {

  char error_buffer[1024];
  u_char **resp;
  std::vector< std::string > out;

  resp = opendmarc_xml_parse((char *)path.c_str(), (char *)&error_buffer[0], (size_t)1024);

  if (resp == NULL) {

    Rcpp::warning(error_buffer);

  } else {

    for (unsigned i = 0; resp[i] != NULL; i++) {
      out.push_back(std::string((char *)resp[i]));
    }

    resp = opendmarc_util_clearargv(resp);

  }

  return(out);

}