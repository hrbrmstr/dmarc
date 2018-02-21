#include <Rcpp.h>
#include <opendmarc/dmarc.h>

using namespace Rcpp;

//' Test if DMARC records are valid
//'
//' @param domains,dmarc_records character vectors of paired domains and DMARC records
//' @export
// [[Rcpp::export]]
std::vector< bool > is_dmarc_valid(std::vector< std::string> domains,
                                   std::vector< std::string> dmarc_records) {

  std::vector< bool > out(dmarc_records.size());

  DMARC_POLICY_T *pctx;
  OPENDMARC_STATUS_T status;

  pctx = opendmarc_policy_connect_init((u_char *)"1.2.3.4", 0);

  for (unsigned int i=0; i<dmarc_records.size(); i++) {

    status = opendmarc_policy_store_dmarc(
      pctx,
      (u_char *)dmarc_records[i].c_str(),
      (u_char *)domains[i].c_str(),
      NULL
    );

    out[i] = (status == DMARC_PARSE_OKAY);

    pctx = opendmarc_policy_connect_rset(pctx);

  }

  pctx = opendmarc_policy_connect_shutdown(pctx);

  return(out);

}

// [[Rcpp::export]]
DataFrame int_read_dmarc(std::vector< std::string> domains) {

  unsigned int dom_size = domains.size();
  std::vector< bool > v_has_valid_dmarc(dom_size);
  std::vector< std::string > v_dkim_domain(dom_size);

  IntegerVector v_pct(dom_size);
  IntegerVector v_adkim(dom_size);
  IntegerVector v_aspf(dom_size);
  IntegerVector v_p(dom_size);
  IntegerVector v_sp(dom_size);

  std::vector< std::string > v_rua(dom_size);
  std::vector< std::string > v_ruf(dom_size);

  DMARC_POLICY_T *pctx;
  OPENDMARC_STATUS_T status;

  pctx = opendmarc_policy_connect_init((u_char *)"1.2.3.4", 0);

  int pct, adkim, aspf, p, sp;
  u_char dkim_domain[1024];
  u_char rua_buf[1024];
  u_char ruf_buf[1024];
  bool ok;

  for (unsigned int i=0; i<domains.size(); i++) {

    status = opendmarc_policy_query_dmarc(pctx, (u_char *)domains[i].c_str());

    ok = (status == DMARC_PARSE_OKAY);

    if (ok) {
      (void)opendmarc_policy_fetch_pct(pctx, &pct);
      (void)opendmarc_policy_fetch_adkim(pctx, &adkim);
      (void)opendmarc_policy_fetch_aspf(pctx, &aspf);
      (void)opendmarc_policy_fetch_p(pctx, &p);
      (void)opendmarc_policy_fetch_sp(pctx, &sp);
      (void)opendmarc_policy_fetch_utilized_domain(pctx, &dkim_domain[0], 1024);
      (void)opendmarc_policy_fetch_rua(pctx, &rua_buf[0], 1024, 1);
      (void)opendmarc_policy_fetch_ruf(pctx, &ruf_buf[0], 1024, 1);
    }

    v_has_valid_dmarc[i] = ok;
    v_dkim_domain[i] = ok ? std::string((char *)dkim_domain) : std::string("");
    v_pct[i] = ok ? pct : NA_INTEGER;
    v_adkim[i] = ok ? adkim : NA_INTEGER;
    v_aspf[i] = ok ? aspf : NA_INTEGER;
    v_p[i] = ok ? p : NA_INTEGER;
    v_sp[i] = ok ? sp : NA_INTEGER;
    v_rua[i] = ok ? std::string((char *)rua_buf) : std::string("");
    v_ruf[i] = ok ? std::string((char *)ruf_buf) : std::string("");

    pctx = opendmarc_policy_connect_rset(pctx);

  }

  pctx = opendmarc_policy_connect_shutdown(pctx);

  DataFrame out = DataFrame::create(
    _["has_valid_dmarc"] = v_has_valid_dmarc,
    _["dkim_domain"] = v_dkim_domain,
    _["pct"] = v_pct,
    _["adkim"] = v_adkim,
    _["aspf"] = v_aspf,
    _["p"] = v_p,
    _["sp"] = v_sp,
    _["rua"] = v_rua,
    _["ruf"] = v_ruf
  );

  out.attr("class") = CharacterVector::create("tbl_df", "tbl", "data.frame");

  return(out);

}

// [[Rcpp::export]]
DataFrame int_parse_dmarc(std::vector< std::string> domains,
                      std::vector< std::string> dmarc_records) {

  unsigned int dom_size = domains.size();
  std::vector< bool > v_has_valid_dmarc(dom_size);
  std::vector< std::string > v_dkim_domain(dom_size);

  IntegerVector v_pct(dom_size);
  IntegerVector v_adkim(dom_size);
  IntegerVector v_aspf(dom_size);
  IntegerVector v_p(dom_size);
  IntegerVector v_sp(dom_size);

  std::vector< std::string > v_rua(dom_size);
  std::vector< std::string > v_ruf(dom_size);

  DMARC_POLICY_T *pctx;
  OPENDMARC_STATUS_T status;

  pctx = opendmarc_policy_connect_init((u_char *)"1.2.3.4", 0);

  int pct, adkim, aspf, p, sp;
  u_char dkim_domain[1024];
  u_char rua_buf[1024];
  u_char ruf_buf[1024];
  bool ok;

  for (unsigned int i=0; i<domains.size(); i++) {

    status = opendmarc_policy_store_dmarc(
      pctx,
      (u_char *)dmarc_records[i].c_str(),
      (u_char *)domains[i].c_str(),
      NULL
    );

    ok = (status == DMARC_PARSE_OKAY);

    if (ok) {
      (void)opendmarc_policy_fetch_pct(pctx, &pct);
      (void)opendmarc_policy_fetch_adkim(pctx, &adkim);
      (void)opendmarc_policy_fetch_aspf(pctx, &aspf);
      (void)opendmarc_policy_fetch_p(pctx, &p);
      (void)opendmarc_policy_fetch_sp(pctx, &sp);
      (void)opendmarc_policy_fetch_utilized_domain(pctx, &dkim_domain[0], 1024);
      (void)opendmarc_policy_fetch_rua(pctx, &rua_buf[0], 1024, 1);
      (void)opendmarc_policy_fetch_ruf(pctx, &ruf_buf[0], 1024, 1);
    }

    v_has_valid_dmarc[i] = ok;
    v_dkim_domain[i] = ok ? std::string((char *)dkim_domain) : std::string("");
    v_pct[i] = ok ? pct : NA_INTEGER;
    v_adkim[i] = ok ? adkim : NA_INTEGER;
    v_aspf[i] = ok ? aspf : NA_INTEGER;
    v_p[i] = ok ? p : NA_INTEGER;
    v_sp[i] = ok ? sp : NA_INTEGER;
    v_rua[i] = ok ? std::string((char *)rua_buf) : std::string("");
    v_ruf[i] = ok ? std::string((char *)ruf_buf) : std::string("");

    pctx = opendmarc_policy_connect_rset(pctx);

  }

  pctx = opendmarc_policy_connect_shutdown(pctx);

  DataFrame out = DataFrame::create(
    _["has_valid_dmarc"] = v_has_valid_dmarc,
    _["dkim_domain"] = v_dkim_domain,
    _["pct"] = v_pct,
    _["adkim"] = v_adkim,
    _["aspf"] = v_aspf,
    _["p"] = v_p,
    _["sp"] = v_sp,
    _["rua"] = v_rua,
    _["ruf"] = v_ruf
  );

  out.attr("class") = CharacterVector::create("tbl_df", "tbl", "data.frame");

  return(out);

}