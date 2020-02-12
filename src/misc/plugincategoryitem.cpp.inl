    // Return the given category item, after having created it, if it didn't
    // already exist

    PluginItem *res = mCategoryItems.value(pCategory);

    if (res == nullptr) {
        // No category item exists for the given category, so create one and add
        // it to our data model (and this in the right place)

        bool inserted = false;
        PluginItem *rootItem = mModel->invisibleRootItem();
        QString categoryName = pluginCategoryName(pCategory);
        QString nonDiacriticCategoryName = nonDiacriticString(categoryName);

        res = new PluginItem(categoryName);

        for (int i = 0, iMax = rootItem->childCount(); i < iMax; ++i) {
            if (nonDiacriticCategoryName < nonDiacriticString(rootItem->child(i)->name())) {
                inserted = true;

                mModel->invisibleRootItem()->insert(i, res);

                break;
            }
        }

        if (!inserted) {
            mModel->invisibleRootItem()->append(res);
        }

        // Keep track of the relationship between our new item and its category

        mCategoryItems.insert(pCategory, res);
        mItemCategories.insert(res, pCategory);
    }

    return res;
