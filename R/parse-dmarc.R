#' Parse already retrieved DMARC records
#'
#' @param domains,dmarc_records character vector of domain names and associated
#'        DMARC records.
#' @export
parse_dmarc <- function(domains, dmarc_records) {

  xdf <- int_parse_dmarc(domains, dmarc_records)

  xdf$adkim <- factor(xdf$adkim, levels=c(0, 114, 115), labels=c("unspecified", "relaxed", "strict"))
  xdf$aspf <- factor(xdf$aspf, levels=c(0, 114, 115), labels=c("unspecified", "relaxed", "strict"))

  xdf$p <- factor(xdf$p, levels=c(0, 110, 113, 114), labels=c("unspecified", "none", "quarantine", "reject"))
  xdf$sp <- factor(xdf$sp, levels=c(0, 110, 113, 114), labels=c("unspecified", "none", "quarantine", "reject"))

  xdf

}
