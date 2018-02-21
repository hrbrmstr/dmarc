#' Parse a DMARC XML report
#'
#' @param x path to report
#' @export
#' @examples
#' dmarc_parse_xml_report(system.file("extdata", "report.xml", package="dmarc"))
dmarc_parse_xml_report <- function(x) {

  x <- path.expand(x)

  res <- parse_report_from_file_int(x)

  return(res)

  if (length(res) > 0) {
    read.csv(
      text = paste0(res, collapse="\n"),
      header = TRUE,
      row.names = NULL,
      stringsAsFactors = FALSE
    ) -> ret
  } else {
    ret <- data.frame()
  }

  class(res) <- c("tbl_df", "tbl", "data.frame")
  return(res)

}