#' Read DMARC records for a set of domains
#'
#' @param domains character vector of domain names
#' @export
read_dmarc <- function(domains) {

  xdf <- int_read_dmarc(domains)

  xdf$adkim <- factor(xdf$adkim, levels=c(0, 114, 115), labels=c("unspecified", "relaxed", "strict"))
  xdf$aspf <- factor(xdf$aspf, levels=c(0, 114, 115), labels=c("unspecified", "relaxed", "strict"))

  xdf$p <- factor(xdf$p, levels=c(0, 110, 113, 114), labels=c("unspecified", "none", "quarantine", "reject"))
  xdf$sp <- factor(xdf$sp, levels=c(0, 110, 113, 114), labels=c("unspecified", "none", "quarantine", "reject"))

  xdf

}
