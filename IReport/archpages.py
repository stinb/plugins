# A library file for paged reports based on architectures

def pageslist(arch, entpages=False, entpagekinds=None):
  pages = []
  pages.append(arch.longname())
  # Page each child architecture
  for child in descendents(arch):
    pages.append(child.longname())

  # Page for each applicable entity
  if entpages:
    pages.extend(entitypages(arch.ents(True), entpagekinds))
  return pages

def entitypages(entlist, entpagekinds=None):
  pages = []
  visited = set()
  for ent in entlist:
    if ent in visited or (entpagekinds and not ent.kind().check(entpagekinds)):
      continue
    visited.add(ent)
    pages.append(str(ent.id()))
  return pages

def breadcrumbs(report, targetarch, page):
    start = targetarch.longname().split('/')
    arch = targetarch
    ent = None
    if page and page.isdigit():
      ent = report.db().ent_from_id(int(page))
      if ent:
        for a in report.db().archs(ent):
          if a.longname().startswith(targetarch.longname()):
            arch = a
            break;
    elif page and targetarch.longname() != page:
      for child in descendents(targetarch):
        if child.longname() == page:
          arch = child
          break

    all = arch.longname().split('/')
    pageIdList = []
    pageNameList = []
    for i in range(len(start)-1, len(all)):
      pageIdList.append('/'.join(all[0:i+1]))
      pageNameList.append(all[i])
    if ent:
      pageIdList.append(str(ent.id()))
      pageNameList.append(ent.name())
    report.breadcrumbs(pageIdList, len(pageIdList) - 1, pageNameList)

    # Return the target
    if ent:
      return ent
    return arch

def descendents(arch):
  all = []
  for child in arch.children():
    all.append(child)
    all.extend(descendents(child))
  return all
